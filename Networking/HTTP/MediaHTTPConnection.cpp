/*
 * Copyright (C) 2013 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define LOG_TAG "MediaHTTPConnection"
#include <utils/Log.h>
#include <media/MediaHTTPConnection.h>
#include <media/stagefright/foundation/ADebug.h>
#include <media/stagefright/MediaErrors.h>
#include <https_internal.h>

using namespace std;
using namespace android;

namespace MagicLeap {
namespace Media {

//HTTP error codes
const static long kHTTP_OK            = 200; //HTTP OK
const static long kHTTP_PARTIAL       = 206; //Partial Content
const static long kHTTP_MULT_CHOICE   = 300; //Multiple Choices
const static long kHTTP_MOVED_PERM    = 301; //Moved Permanently
const static long kHTTP_MOVED_TEMP    = 302; //Moved Temporarily
const static long kHTTP_SEE_OTHER     = 303; //See Other
const static long kHTTP_TEMP_REDIRECT = 307; //Temporary Redirect
const static long kHTTP_FORBIDDEN     = 403; //Forbidden
const static long kHTTP_REQUESTED_RANGE_NOT_SATISFIABLE = 416; //Invalid range

//Max redirect attempts
const static int MAX_REDIRECTS = 20;

//Connection timeout - 30 seconds
const uint32_t kConnectionTimeout = 30;

//To maintain all the Cookies for this session
static string gSessionCookie;

//A class that helps with Initializing and Cleaning up HTTPS library per process
struct LibLoadUnload {
    LibLoadUnload() {
        ALOGI("LibLoadUnload: Initializing HTTPS library");
        HttpsLibInit();
        gSessionCookie.clear();
    }

    ~LibLoadUnload() {
        ALOGI("LibLoadUnload: Cleaning up HTTPS library");
        HttpsLibFini();
        gSessionCookie.clear();
    }
};
static LibLoadUnload obj;

//HTTP Request Status Callback
void ProcessHttpsResponseStatusCallback(HttpsSingleStatus* pStatus, void* p)
{
    MediaHTTPConnection* pSelf = (MediaHTTPConnection*)p;

    if (pSelf) {
        pSelf->processResponseStatus(pStatus);
    }

    return;
}

//HTTP Request Header Info Callback
size_t ProcessHttpsHeaderCallback(char* pHeader, size_t uSize, size_t uCount, void* p)
{
    size_t retSize = 0;
    MediaHTTPConnection* pSelf = (MediaHTTPConnection*)p;

    if (pSelf) {
        retSize = pSelf->processHttpsHeader(pHeader, uSize, uCount);
    }

    return retSize;
}

//HTTP Request Response Body Callback
size_t ProcessHttpsBodyCallback(void* pBody, size_t uSize, size_t uCount, void* p)
{
    size_t retSize = 0;
    MediaHTTPConnection* pSelf = (MediaHTTPConnection*)p;

    if (pSelf) {
        retSize = pSelf->processHttpsBody((char*)pBody, uSize, uCount);
    }

    return retSize;
}

//Enable this for HTTP connection trace log
//#define DEBUG_TRACE 1

#ifdef DEBUG_TRACE

//Enable one of the below for level of HTTP connection trace log
//#define DEBUG_LEVEL 1  //Info only - by default it will be enabled
//#define DEBUG_LEVEL 2  //Info + Header
//#define DEBUG_LEVEL 3  //Info + Header + Data
#define DEBUG_LEVEL 4  //Info + Header + Data + SSL

// HTTP debug trace
typedef struct {
    FILE*        pFile;
    uint32_t     uTraceInfoMask;
    HttpsSingle* pSingle;
    struct timespec begin;
    struct timespec end;
} DebugInfo;

//HTTP connection trace dump file
const char* gDbgFile = "/data/local/tmp/http_trace.log";
DebugInfo gDbgInfo = {NULL, 0, NULL, {0, 0}, {0, 0}};

static void HelperDebugTraceDump(const char* text, FILE* pFile,
                                 unsigned char* ptr, size_t size)
{
    uint32_t width = 0x10;
    size_t   i = 0;
    size_t   c = 0;
    char     x = 0;

    fprintf(pFile, "%s, %10.10zu bytes (0x%8.8zx)\n", text, size, size);

    for (i = 0; i < size; i += width) {
        //Write offset first
        fprintf(pFile, "%4.4zx: ", i);

        //Write as hex string next
        for (c = 0; c < width; c++) {
            if (i + c < size) {
                fprintf(pFile, "%02x ", ptr[i+c]);
            } else {
                fputs("   ", pFile);
            }
        }

        //Write as char string now
        for (c = 0; (c < width) && (i + c < size); c++) {
            x = (ptr[i + c] >= 0x20 && ptr[i + c] < 0x80) ? ptr[i + c] : '.';
            fputc(x, pFile);
        }

        fputc('\n', pFile);
    }
}

//debug trace callback
static int DebugTraceCallback(void* /*handle*/, trace_info_type type,
             char* data, size_t size, void* userp)
{
    const char* text = NULL;
    DebugInfo*  pDbgInfo = (DebugInfo*)userp;
    bool        bDone = true;

    if (pDbgInfo && data) {
        switch (type) {
        case TRACE_INFO_TEXT:
            if (pDbgInfo->uTraceInfoMask & TRACE_INFO_MASK_TEXT) {
                if (pDbgInfo->pFile) {
                    fprintf(pDbgInfo->pFile, "== Info: %s", data);
                }
            }
            break;
        case TRACE_INFO_HEADER_OUT:
            if (pDbgInfo->uTraceInfoMask & TRACE_INFO_MASK_HEADER_OUT) {
                text = "=> Send header";
                bDone = false;
            }
            break;
        case TRACE_INFO_DATA_OUT:
            if (pDbgInfo->uTraceInfoMask & TRACE_INFO_MASK_DATA_OUT) {
                text = "=> Send data";
                bDone = false;
            }
            break;
        case TRACE_INFO_SSL_DATA_OUT:
            if (pDbgInfo->uTraceInfoMask & TRACE_INFO_MASK_SSL_DATA_OUT) {
                text = "=> Send SSL data";
            }
            break;
        case TRACE_INFO_HEADER_IN:
            if (pDbgInfo->uTraceInfoMask & TRACE_INFO_MASK_HEADER_IN) {
                text = "<= Recv header";
                bDone = false;
            }
            break;
        case TRACE_INFO_DATA_IN:
            if (pDbgInfo->uTraceInfoMask & TRACE_INFO_MASK_DATA_IN) {
                text = "<= Recv data";
                bDone = false;
            }
            break;
        case TRACE_INFO_SSL_DATA_IN:
            if (pDbgInfo->uTraceInfoMask & TRACE_INFO_MASK_SSL_DATA_IN) {
                text = "<= Recv SSL data";
                bDone = false;
            }
            break;
        default:
            break;
        }

        if (bDone == false && pDbgInfo->pFile) {
            HelperDebugTraceDump(text, pDbgInfo->pFile, (unsigned char*)data, size);
        }
    }
    return 0;
}

static void SetupDebugLogs(const char* uri)
{
    memset(&gDbgInfo, 0, sizeof(gDbgInfo));
    //Create an empty file now
    gDbgInfo.pFile = fopen(gDbgFile, "w");
    if (gDbgInfo.pFile) {
        ALOGD("%s: Debug Trace file opened at [%s]", __FUNCTION__, gDbgFile);
    } else {
        ALOGD("%s: Debug Trace file Failed for [%s]", __FUNCTION__, gDbgFile);
        return;
    }
    gDbgInfo.uTraceInfoMask = TRACE_INFO_MASK_TEXT;
#if DEBUG_LEVEL >= 2
    gDbgInfo.uTraceInfoMask |= TRACE_INFO_MASK_HEADER_OUT | TRACE_INFO_MASK_HEADER_IN;
#endif
#if DEBUG_LEVEL >= 3
    gDbgInfo.uTraceInfoMask |= TRACE_INFO_MASK_DATA_OUT | TRACE_INFO_MASK_DATA_IN;
#endif
#if DEBUG_LEVEL >= 4
    gDbgInfo.uTraceInfoMask |= TRACE_INFO_MASK_SSL_DATA_OUT | TRACE_INFO_MASK_SSL_DATA_IN;
#endif
    if (gDbgInfo.pFile) {
        fprintf(gDbgInfo.pFile, "HTTP Connection Trace for [%s]\n", uri);
        fprintf(gDbgInfo.pFile, "=========================================================\n");
        fclose(gDbgInfo.pFile);
        gDbgInfo.pFile = NULL;
    }

    clock_gettime(CLOCK_MONOTONIC_RAW, &gDbgInfo.begin);
}

static void UpdateDebugLogs(HttpsSingle* pHttpsSingle, int redirectCount)
{
    if (gDbgInfo.pFile == NULL) {
        //Open the file in append mode
        gDbgInfo.pFile = fopen(gDbgFile, "a");
    }
    if (redirectCount > 0 && gDbgInfo.pFile) {
        pHttpsSingle->SetDebugTraceCallback(pHttpsSingle, DebugTraceCallback, (void*)&gDbgInfo);
        fprintf(gDbgInfo.pFile, "=========================================================\n");
        fprintf(gDbgInfo.pFile, "Redirection Count: %d\n", redirectCount);
        fprintf(gDbgInfo.pFile, "=========================================================\n");
    }
}

static void CloseDebugLogs()
{
    if (gDbgInfo.pFile) {
        clock_gettime(CLOCK_MONOTONIC_RAW, &gDbgInfo.end);
        unsigned long delta_us = (gDbgInfo.end.tv_sec - gDbgInfo.begin.tv_sec) * 1000000;
        delta_us += (gDbgInfo.end.tv_nsec - gDbgInfo.begin.tv_nsec) / 1000;
        fprintf(gDbgInfo.pFile, "took %lu micro-seconds", delta_us);
        fprintf(gDbgInfo.pFile, "=========================================================\n");
        fclose(gDbgInfo.pFile);
        gDbgInfo.pFile = NULL;
    }
}
#else
#define SetupDebugLogs(...)
#define UpdateDebugLogs(...)
#define CloseDebugLogs(...)
#endif

//MediaHTTPConnection constructor
MediaHTTPConnection::MediaHTTPConnection() :
    mConnectionState(EDISCONNECTED),
    mHttpsSingle(NULL),
    mURL(),
    mContentType(),
    mContentRange(),
    mResponseCode(kHTTP_OK),
    mContentTotalLength(-1),
    mCurrentOffset(-1),
    mCurrentSize(0),
    mMemory(NULL),
    mFirstConnectionDone(false),
    mTriedWithCookie(false)
{
    HttpsMultiInit(&mHttpsMulti);
    //Enable HTTP (along with HTTPS)
    HttpsEnablePlainHTTP(1);
}

//MediaHTTPConnection destructor
MediaHTTPConnection::~MediaHTTPConnection()
{
    //Do Cleanup now
    disconnect();
    mHttpsMulti.Cleanup(&mHttpsMulti);
}

void MediaHTTPConnection::closeConnection()
{
    //disconnect this http connection and clean up the status
    if (mConnectionState != EDISCONNECTED) {
        mConnectionState = EDISCONNECTED;
        mHttpsSingle = NULL;
        mContentRange = "";
        mResponseCode = kHTTP_OK;
        mCurrentOffset = -1;
        mCurrentSize = 0;
        mTriedWithCookie = false;
        if (mMemory) {
            free(mMemory);
            mMemory = NULL;
        }
    }
}

//HTTP Disconnection Method
void MediaHTTPConnection::disconnect()
{
    Mutex::Autolock lock(mLock);
    disconnect_l();
}

void MediaHTTPConnection::disconnect_l()
{
    closeConnection();
    mURL = "";
    mContentType = "";
    mContentTotalLength = -1;
    mFirstConnectionDone = false;
    mHeaders.clear();
}

void MediaHTTPConnection::copyHttpsHeaders(const HeaderVector* headers)
{
    size_t uCount = 0;
    size_t index = 0;

    if (headers) {
        uCount = headers->size();
        for (index = 0; index < uCount; index++) {
            mHeaders.add(headers->keyAt(index), headers->valueAt(index));
        }
    }
}

//HTTP Connection Method
bool MediaHTTPConnection::connect(const char* uri, const HeaderVector* headers)
{
    bool bSuccess = false;
    Mutex::Autolock lock(mLock);

    disconnect_l();

    if (uri) {
        copyHttpsHeaders(headers);
        mURL = uri;
        bSuccess = true;
        SetupDebugLogs(uri);
    }

    return bSuccess;
}

HttpsCode MediaHTTPConnection::setHttpsHeaders(off64_t aBeginRange, ssize_t aSize)
{
    HttpsCode retCode = NOVA_NET_ERR_OK;
    String8   field;
    size_t    uCount = 0;
    size_t    index = 0;
    long long rangeOffset = aBeginRange;

    uCount = mHeaders.size();
    for (index = 0; index < uCount; index++) {
        field = mHeaders.keyAt(index);
        field.append(": ");
        field.append(mHeaders.valueAt(index));
        retCode |= mHttpsSingle->SetHeaderField(mHttpsSingle, field.string());
    }
    //Add Range header, if its a valid range
    if (aBeginRange >= 0) {
        field = "Range: bytes=";
        field.appendFormat("%lld-", rangeOffset);
        //If we know how much of content we want to read, add that as well
        //This will optimize the read operation
        //NOTE that, the binder based implementation lacks this optimization
        if (aSize >= 0) {
            if (aSize > 0) {
                //Offset is begin-index:end-index. So set the index (not size)
                aSize--;
            }
            rangeOffset += aSize;
            field.appendFormat("%lld", rangeOffset);
        }
        retCode |= mHttpsSingle->SetHeaderField(mHttpsSingle, field.string());
    }

    //Add Cookies if we have
    if (!gSessionCookie.empty()) {
        field = "Cookie: ";
        field.append(gSessionCookie.c_str());
        retCode |= mHttpsSingle->SetHeaderField(mHttpsSingle, field.string());
    }

    return retCode;
}

bool MediaHTTPConnection::makeHTTPSRequest(off64_t aRange, ssize_t aSize)
{
    bool      bSuccess = false;
    int       redirectCount = 0;
    string    url(mURL);
    string    lastURL(mURL);
    HttpsCode retCode = NOVA_NET_ERR_OK;
    HttpRequestType method = HTTP_GET;

    closeConnection();

    do {
        mHttpsSingle = mHttpsMulti.InitSingle(&mHttpsMulti);
        if (!mHttpsSingle) {
            ALOGE("%s(%d): HttpsMulti.InitSingle failed to create HttpsSingle object!!",
                   __FUNCTION__, __LINE__);
            retCode = UNKNOWN_ERROR;
            break;
        }

        retCode |= mHttpsSingle->SetURL(mHttpsSingle, url.c_str());
        retCode |= mHttpsSingle->SetConnectionTimeout(mHttpsSingle, kConnectionTimeout);
        retCode |= mHttpsSingle->SetRequestMethod(mHttpsSingle, method);
        retCode |= setHttpsHeaders(aRange, aSize);
        retCode |= mHttpsSingle->SetResponseStatusCallback(mHttpsSingle,
                                   ProcessHttpsResponseStatusCallback, this);
        retCode |= mHttpsSingle->SetResponseHeaderCallback(mHttpsSingle,
                                   ProcessHttpsHeaderCallback, this);
        retCode |= mHttpsSingle->SetResponseBodyCallback(mHttpsSingle,
                                   ProcessHttpsBodyCallback, this);

        UpdateDebugLogs(mHttpsSingle, redirectCount);

        retCode |= mHttpsMulti.AddSingle(&mHttpsMulti, mHttpsSingle);
        if (retCode == NOVA_NET_ERR_OK) {
            //Blocking call - 2nd argument means, one time request
            retCode = mHttpsMulti.ExecRequestsHighPerformance(&mHttpsMulti, 0);
            if (retCode != NOVA_NET_ERR_OK) {
                //Something went wrong - break the loop!
                ALOGE("%s(%d): Something went wrong (%d) with the HTTP Request",
                       __FUNCTION__, __LINE__, (int)retCode);
                break;
            }
            if (mResponseCode == kHTTP_FORBIDDEN && !mTriedWithCookie) {
                ALOGV("%s: HTTP ResponseCode: 403 Forbidden! Try with cookies now", __FUNCTION__);
                mTriedWithCookie = true;
                continue;
            }
            if (mResponseCode != kHTTP_MULT_CHOICE && mResponseCode != kHTTP_MOVED_PERM &&
                mResponseCode != kHTTP_MOVED_TEMP && mResponseCode != kHTTP_SEE_OTHER &&
                mResponseCode != kHTTP_TEMP_REDIRECT) {
                //All good - so break the loop
                break;
            }
            //Set the updated URL
            url = mURL;
            if (mResponseCode != kHTTP_TEMP_REDIRECT) {
                //update effective URL, unless it is a Temporary Redirect
                lastURL = url;
            }
        } else {
            //Something went wrong - break the loop!
            ALOGE("%s(%d): Something went wrong (%d) with the HTTP Request",
                   __FUNCTION__, __LINE__, (int)retCode);
            break;
        }
    } while (++redirectCount < MAX_REDIRECTS);

    CloseDebugLogs();
    //Check whether the HTTP Request was success
    if (retCode == NOVA_NET_ERR_OK && redirectCount < MAX_REDIRECTS) {
        bSuccess = true;
        mConnectionState = ECONNECTED;
        if (mFirstConnectionDone == false) {
            //First time: Update all the necessary information now
            getInfo(ECONTENTLENGTH, NULL);
            getInfo(ECONTENTTYPE, &mContentType);
            mFirstConnectionDone = true;
        }
        mURL = lastURL;

        if (aRange > 0 && mResponseCode != kHTTP_PARTIAL) {
            if (aRange >= mContentTotalLength &&
                mResponseCode == kHTTP_REQUESTED_RANGE_NOT_SATISFIABLE) {
                //Its an illegal range (beyond content length) ==> EOS - So don't fail
                ALOGI("%s: Reached EOS(%jd:%lld)", __FUNCTION__, aRange, mContentTotalLength);
            } else if (mResponseCode == kHTTP_OK) {
                //Ideally this shouldn't happen - but some servers may respond with HTTP OK
                mCurrentOffset = aRange;
            } else {
                //Some servers simply ignore "Range" requests and serve
                //data from the start of the content.
                ALOGE("%s(%d): Server Ignored Range(%jd-%zd) request!!",
                       __FUNCTION__, __LINE__, aRange, aSize);
                bSuccess = false;
            }
        } else {
            mCurrentOffset = aRange;
        }
    } else {
        mConnectionState = ECONNECTIONFAILED;
        ALOGE("%s: Failed to Connect retCode(%d) mResponseCode(%d) redirectCount(%d)",
           __FUNCTION__, (int)retCode, (int)mResponseCode, redirectCount);
    }

    return bSuccess;
}

//Content-Range format is "bytes xxx-yyy/zzz
//where "zzz" is the total number of bytes of the content or '*' if unknown.
void MediaHTTPConnection::getContentLengthFromRange()
{
    string value;
    size_t index = 0;

    index = mContentRange.rfind('/');
    if (index != string::npos) {
        value = mContentRange.substr(index + 1);
        if (value.find('*') == string::npos) {
            mContentTotalLength = stoll(value);
        }
    }
}

status_t MediaHTTPConnection::getInfo(InfoType type, string* info)
{
    status_t  retCode = UNKNOWN_ERROR;
    HttpsCode httpCode = NOVA_NET_ERR_OK;
    char*     pInfo = NULL;

    if (mConnectionState == ECONNECTED) {
        switch (type) {
        case ECONTENTLENGTH:
            //Some HTTP Servers may respond with HTTP_OK instead of HTTP_PARTIAL
            //In such cases, GetContentLength returns the range (and not length)
            //So its better to relay on "Content-Range" header, if there is one
            if (mContentRange.empty()) {
                httpCode = mHttpsSingle->GetContentLength(mHttpsSingle, &mContentTotalLength);
            } else if (mResponseCode == kHTTP_PARTIAL || mResponseCode == kHTTP_OK) {
                //Partial content, we cannot just use GetContentLength
                //because what we want is not just the length of the range
                //returned but the size of the full content if available.
                getContentLengthFromRange();
            }
            break;
        case ECONTENTTYPE:
            httpCode = mHttpsSingle->GetContentType(mHttpsSingle, &pInfo);
            break;
        case EURI:
            if (mURL.empty()) {
                httpCode = mHttpsSingle->GetLastEffectiveURL(mHttpsSingle, &pInfo);
            } else {
                pInfo = (char*)mURL.c_str();
            }
            break;
        default:
            httpCode = NOVA_NET_ERR_UNSUPPORTED_PROTOCOL;
        }

        if (NOVA_NET_ERR_OK == httpCode) {
            if (info && pInfo) {
                *info = pInfo;
            }
            retCode = OK;
        }
    }

    return retCode;
}

//Get the size of HTTP Response body
off64_t MediaHTTPConnection::getSize()
{
    if (mFirstConnectionDone == false) {
        Mutex::Autolock lock(mLock);
        makeHTTPSRequest();
    }

    return mContentTotalLength;
}

//Get the Mime-type (Content-type) of HTTP Response body
status_t MediaHTTPConnection::getMIMEType(String8* mimeType)
{
    status_t status = UNKNOWN_ERROR;

    if (mFirstConnectionDone == false) {
        Mutex::Autolock lock(mLock);
        makeHTTPSRequest();
    }

    if (mFirstConnectionDone) {
        *mimeType = mContentType.c_str();
        status = OK;
    }
    return status;
}

//Get the final effective URL
status_t MediaHTTPConnection::getUri(String8* uri)
{
    status_t status = UNKNOWN_ERROR;

    if (mFirstConnectionDone == false) {
        Mutex::Autolock lock(mLock);
        makeHTTPSRequest();
    }

    if (mFirstConnectionDone) {
        *uri = mURL.c_str();
        status = OK;
    }
    return status;
}

/*
 * Copy the data from the previous HTTPS request, if there is any.
 */
ssize_t MediaHTTPConnection::copyData(off64_t uOffset, void* pBuffer, size_t uSize) const
{
    off64_t diffOffset_ = 0;
    off64_t currentMemSize = 0;
    ssize_t diffOffset = -1;
    ssize_t availableData = 0;
    ssize_t copyDataSize = ERROR_MALFORMED;

    //If we already have some data downloaded (with the previous HTTPS request),
    //copy that first before making another HTTPS request
    diffOffset_ = uOffset - mCurrentOffset;
    currentMemSize = mCurrentSize;
    if (diffOffset_ < currentMemSize) {
        diffOffset = (ssize_t)diffOffset_;
    }
    if (mConnectionState == ECONNECTED && mMemory && pBuffer &&
        diffOffset >= 0 && mCurrentSize > 0 && uOffset < mContentTotalLength) {
        availableData = mCurrentSize - diffOffset;
        copyDataSize = uSize;
        if ((ssize_t)uSize > availableData) {
            copyDataSize = availableData;
        }
        //Copy the data to read buffer
        memcpy(pBuffer, mMemory + diffOffset, copyDataSize);
    } else if (uOffset >= mContentTotalLength) {
        //EOS
        copyDataSize = 0;
    }

    return copyDataSize;
}

/*
 * Reads max of uSize bytes of HTTPS response body from uOffset bytes
 * Data is written into pBuffer
 */
ssize_t MediaHTTPConnection::readAt(off64_t uOffset, void* pBuffer, size_t uSize)
{
    ssize_t readDataSize = ERROR_MALFORMED;
    ssize_t bufferedRead = 0;
    Mutex::Autolock lock(mLock);

    //Copy buffered data, if any
    bufferedRead = copyData(uOffset, pBuffer, uSize);
    if (bufferedRead > 0) {
        //Advance the read buffer pointer, size, and offset
        pBuffer = ((char*)pBuffer + bufferedRead);
        uSize -= bufferedRead;
        uOffset += bufferedRead;
    } else {
        bufferedRead = 0;
    }

    //Check whether we need to make a HTTPS request
    if (uSize > 0) {
        if (mFirstConnectionDone && (uOffset >= mContentTotalLength)) {
            //Its an illegal range (beyond content length). Return EOS
            readDataSize = 0;
        } else {
            //Issue an HTTP request with data range
            makeHTTPSRequest(uOffset, uSize);
            readDataSize = copyData(uOffset, pBuffer, uSize);
        }
        if (readDataSize > 0) {
            readDataSize += bufferedRead;
        }
    } else if (uSize == 0) {
        readDataSize = bufferedRead;
    }

    return readDataSize;
}

//Process the HTTP request status
void MediaHTTPConnection::processResponseStatus(HttpsSingleStatus* pStatus)
{
    HttpsSingle* pSingle = NULL;

    if (pStatus && pStatus->single) {
        pSingle = (HttpsSingle*)pStatus->single;
        pSingle->GetResponseCode(pSingle, &mResponseCode);
    }
}

//Local helper function for parsing HTTP Response Header
const char* LocationField = "Location: ";
const char* LengthField = "Content-Length: ";
const char* RangeField = "Content-Range: ";
const char* TypeField = "Content-Type: ";
const char* SetCookieField = "Set-Cookie: ";

static bool FindField(const char* pHeader, size_t uHeaderSize,
                      const char* pField, string* pVal)
{
    string headerField(pHeader, uHeaderSize - 2); //Ignore "\r\n"
    size_t uFieldLen = 0;
    size_t index = 0;
    bool   bSuccess = false;

    index = headerField.find(pField);
    if (index != string::npos) {
        uFieldLen = strlen(pField);
        *pVal = headerField.substr(index + uFieldLen, uHeaderSize - uFieldLen);
        bSuccess = true;
    }

    return bSuccess;
}

/*
 * Received HTTPS Response header is uCount * uSize bytes long in 'pHeader'
 * and is not null terminated
 */
size_t MediaHTTPConnection::processHttpsHeader(const char* pHeader, size_t uSize, size_t uCount)
{
    size_t uTotalSize = 0;
    string cookie;

    //We can save Header if needed. But as of now, there is no real requirement
    //Lets just lookout for Content Range & Location and save it for future
    uTotalSize = uSize * uCount;
    FindField(pHeader, uTotalSize, RangeField, &mContentRange);
    FindField(pHeader, uTotalSize, LocationField, &mURL);
    FindField(pHeader, uTotalSize, SetCookieField, &cookie);

    if (!cookie.empty()) {
        //Fetch only the first name-value pair from the cookie field
        size_t index = cookie.find("; ");
        if (index != string::npos) {
            cookie.resize(index);
        }
        //Add the cookie to session cookie list, if its not duplicated
        if (gSessionCookie.find(cookie) == string::npos) {
            if (!gSessionCookie.empty()) {
                gSessionCookie.append("; ");
            }
            gSessionCookie.append(cookie);
        }
    }

    return uTotalSize;
}

/*
 * Received HTTPS Response body is uCount * uSize bytes long in 'pBody'
 * and is not null terminated
 */
size_t MediaHTTPConnection::processHttpsBody(const char* pBody, size_t uSize, size_t uCount)
{
    size_t    uBodySize = 0;
    size_t    uUpdatedSize = 0;
    uint8_t*  pTemp = mMemory;

    //Save the body locally for future read operation
    uBodySize = uSize * uCount;
    uUpdatedSize = mCurrentSize + uBodySize;
    mMemory = (uint8_t*)realloc(pTemp, uUpdatedSize);
    if (mMemory) {
        memcpy(mMemory + mCurrentSize, pBody, uBodySize);
        mCurrentSize = uUpdatedSize;
    } else {
        //ERROR: This shouldn't happen
        uBodySize = 0;
        ALOGE("%s: Failed allocate buffer of [%zu] bytes", __FUNCTION__, uUpdatedSize);
        if (pTemp) {
            free(pTemp);
        }
    }

    return uBodySize;
}

}  // namespace Media
}  // namespace MagicLeap
