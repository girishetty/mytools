// %BANNER_BEGIN%
// ---------------------------------------------------------------------
// %COPYRIGHT_BEGIN%
//
// Copyright (c) 201x Magic Leap, Inc. (COMPANY) All Rights Reserved.
// Magic Leap, Inc. Confidential and Proprietary
//
// NOTICE: All information contained herein is, and remains the property
// of COMPANY. The intellectual and technical concepts contained herein
// are proprietary to COMPANY and may be covered by U.S. and Foreign
// Patents, patents in process, and are protected by trade secret or
// copyright law. Dissemination of this information or reproduction of
// this material is strictly forbidden unless prior written permission is
// obtained from COMPANY. Access to the source code contained herein is
// hereby forbidden to anyone except current COMPANY employees, managers
// or contractors who have executed Confidentiality and Non-disclosure
// agreements explicitly covering such access.
//
// The copyright notice above does not evidence any actual or intended
// publication or disclosure of this source code, which includes
// information that is confidential and/or proprietary, and is a trade
// secret, of COMPANY. ANY REPRODUCTION, MODIFICATION, DISTRIBUTION,
// PUBLIC PERFORMANCE, OR PUBLIC DISPLAY OF OR THROUGH USE OF THIS
// SOURCE CODE WITHOUT THE EXPRESS WRITTEN CONSENT OF COMPANY IS
// STRICTLY PROHIBITED, AND IN VIOLATION OF APPLICABLE LAWS AND
// INTERNATIONAL TREATIES. THE RECEIPT OR POSSESSION OF THIS SOURCE
// CODE AND/OR RELATED INFORMATION DOES NOT CONVEY OR IMPLY ANY RIGHTS
// TO REPRODUCE, DISCLOSE OR DISTRIBUTE ITS CONTENTS, OR TO MANUFACTURE,
// USE, OR SELL ANYTHING THAT IT MAY DESCRIBE, IN WHOLE OR IN PART.
//
// %COPYRIGHT_END%
// --------------------------------------------------------------------
// %BANNER_END%
//

#include <stdio.h>
#include <string.h>
#include <string>
#include <openssl/md5.h>
#include "MediaHTTPConnection.h"

using namespace std;

//Enable this to directly try with NOVA HTTPS library
//#define NOVA
//Enable this to dowload contents into a file
#define FILE_WRITE

#define DATA_DEFAULT_SIZE 0x2000

#ifdef FILE_WRITE
const char* gDowloadFile  = "/home/gshetty/Docs/HTTPS/src/download.file";
const char* gDowloadFile_ = "/home/gshetty/Docs/HTTPS/src/download.file_";
#else
void DumpData(const char* pData, off64_t uSize)
{
    printf("Content[%zu]\n", uSize);
    printf("%s\n", pData);
}
#endif

void AddHeaderField(KeyedVector<string, string>* pHeader, const char* pInfo)
{
    string name;
    string  name_;
    string value;
    string  value_;
    size_t  index = 0;
    string  info(pInfo);

    if (pHeader && pInfo) {
        index = info.find(": ");
        if (index > 0) {
            name_ = info.substr(0, index);
            value_ = info.substr(index + 2, std::string::npos);
            name = name_.c_str();
            value = value_.c_str();
            pHeader->add(name, value);
        }
    }
}

void DownloadPage(const char* pUrl)
{
    off64_t  contentLen = 0;
    off64_t  offset = 0;
    ssize_t  retLen = 0;
    char*    pData = NULL;
    status_t status = OK;
    string   mimeType;
    string   uri;
    size_t   dataSize = 0;
#ifdef FILE_WRITE
    FILE* fp = fopen(gDowloadFile, "w");
#endif

    printf("\nDownloadPage: URL: [%s]\n", pUrl);
    KeyedVector<string, string> headers;
    MediaHTTPConnection* httpCon = new MediaHTTPConnection;

    if (httpCon) {
        AddHeaderField(&headers, "Connection: keep-alive");
        if (httpCon->connect(pUrl, &headers)) {
            status = httpCon->getMIMEType(&mimeType);
            if (OK == status) {
                printf("DownloadPage: MimeType(%s)\n", mimeType.c_str());
            }
            status = httpCon->getUri(&uri);
            if (OK == status) {
                printf("DownloadPage: Effective URI(%s)\n", uri.c_str());
            }
            contentLen = httpCon->getSize();
            printf("DownloadPage: Content Length(%lld)\n", (long long)contentLen);
            if (contentLen > 0) {
                pData = new char [DATA_DEFAULT_SIZE];
                if (pData) {
                    do {
                        dataSize = DATA_DEFAULT_SIZE;
                        if ((off64_t)dataSize > contentLen) {
                            dataSize = (size_t)contentLen;
                        }
                        retLen = httpCon->readAt(offset, pData, dataSize);
                        //printf("DownloadPage: Read Length(%ld)\n", retLen);
                        if (retLen >= 0 && retLen <= dataSize) {
                            contentLen -= retLen;
                            offset += retLen;
#ifdef FILE_WRITE
                            if (fp) {
                                fwrite(pData, retLen, 1, fp);
                            }
#else
                            DumpData(pData, retLen);
#endif
                        } else {
                            break;
                        }
                    } while (contentLen!= 0);
                    delete pData;
                }
            }
        } else {
            printf("Connect Failed\n");
        }
        httpCon->disconnect();
        delete httpCon;
    }

#ifdef FILE_WRITE
    if (fp) {
        fclose(fp);
    }
#endif
}

#ifdef NOVA
const char* const LengthField = "Content-Length: ";
const char* const TypeField = "Content-Type: ";

bool FindField(const char* pHeader, size_t uSize, const char* const pField, string* pVal)
{
    string headerField(pHeader, uSize);
    size_t uFieldLen = 0;
    size_t index = 0;
    bool   bSuccess = false;

    index = headerField.find(pField);
    if (index != string::npos) {
        uFieldLen = strlen(pField);
        *pVal = headerField.substr(index + uFieldLen, uSize - uFieldLen);
        pVal->resize(pVal->size() - 2);
        bSuccess = true;
    }

    return bSuccess;
}
    
size_t gContentLen = 0;
string gContentType;

static size_t header_callback(char* buffer, size_t size,
                              size_t nitems, void* /*userdata*/)
{
    size_t uTotalLen = 0;
    size_t uVal = 0;
    string field;

    /* received header is nitems * size long in 'buffer' NOT ZERO TERMINATED */
    /* 'userdata' is set with CURLOPT_HEADERDATA */
    uTotalLen = nitems * size;
    //printf("header_callback: %zu * %zu = %zu: %s",  nitems, size, uTotalLen, buffer);
    printf("%s", buffer);

    if (FindField(buffer, uTotalLen, LengthField, &field)) {
        gContentLen = atoi(field.c_str());
        //printf("header_callback: %s [%s][%zu]\n", LengthField, field.c_str(), gContentLen);
    } else if (FindField(buffer, uTotalLen, TypeField, &field)) {
        gContentType = field;
        //printf("header_callback: %s [%s]\n", TypeField, field.c_str());
    }
    return uTotalLen;
}

static size_t body_callback(void* buffer, size_t size,
                              size_t nitems, void* userdata)
{
    FILE* fp = (FILE*)userdata;
    /* received body is nitems * size long in 'buffer' NOT ZERO TERMINATED */
    /* 'userdata' is set with CURLOPT_HEADERDATA */
    if (fp) {
#ifdef FILE_WRITE
        fwrite(buffer, size, nitems, fp);
#endif
    } else {
        printf("body_callback: %zu: [%s]\n",  nitems * size, (char*)buffer);
    }
    return nitems * size;
}

/// example HTTP status callback
static void requeststatus_callback(HttpsSingleStatus* pStatus, const void* /*userp*/)
{
    long response_code = 0;
    long long content_length = 0;
    char* content_type = NULL;
    char* last_effective_URL = NULL;

    HttpsSingle* pSingle = (HttpsSingle*)pStatus->single;
    if (pSingle) {
      pSingle->GetResponseCode(pSingle, &response_code);
      printf("response code %ld\n", response_code);
      pSingle->GetContentLength(pSingle, &content_length);
      printf("content length %lld\n", content_length);
      pSingle->GetContentType(pSingle, &content_type);
      printf("content type: %s\n", content_type);
      pSingle->GetLastEffectiveURL(pSingle, &last_effective_URL);
      printf("last effective URL %s\n", last_effective_URL);
    }
}

void DownloadPageWithNova(const char* pUrl)
{
    HttpsCode ret;
    HttpsMulti multi;
    HttpsSingle single;
#ifdef FILE_WRITE
    FILE* fp = fopen(gDowloadFile_, "w");
#else
    FILE* fp = NULL;
#endif

    printf("\nDownloadPageWithNova: URL: [%s]\n", pUrl);
    HttpsMultiInit(&multi);
    HttpsSingleInit(&single);

    single.SetURL(&single, pUrl);
    single.SetRequestMethod(&single, HTTP_GET);

    //testing custimize HTTP header
    single.SetHeaderField(&single, "Connection: keep-alive");
    single.SetHeaderField(&single, "Range: bytes=0-");
    single.SetResponseBodyCallback(&single, body_callback, fp);
    single.SetResponseHeaderCallback(&single, header_callback, NULL);
    single.SetRequestStatusCallback(&single, requeststatus_callback, NULL);

    multi.AddSingle(&multi, &single);
    ret = multi.ExecRequests(&multi);
    if (ret != NOVA_NET_ERR_OK) {
        printf("failed to exec requests %d\n", ret);
    }

    printf("Content-Type: [%s]\n", gContentType.c_str());
    printf("Content-Length: [%zu]\n", gContentLen);
#ifdef FILE_WRITE
    if (fp) {
        fclose(fp);
    }
#endif
    single.Cleanup(&single);
    multi.Cleanup(&multi);
}
#endif

#define TEST_URL_COUNT 7

const char* gUrlList[TEST_URL_COUNT] = {
    "http://10.11.50.200/Media/Cleopatra%20Stratan%20-%20Zunea-Zunea.mp3",
    "http://10.11.50.200/Media/Adele%E2%80%93Send_My_Love_(Mp3Nobs.com).mp4",
    "http://10.11.50.200/Media/SampleVideo_360x240_30mb.mp4",
    "http://10.11.50.200/Media/big_buck_bunny_360p_30mb.mp4",
    "http://10.11.50.200/Media/big_buck_bunny_480p_30mb.mp4",
    "http://10.11.50.200/Media/SampleVideo_1280x720_30mb.mp4",
    "http://www.example.com"
};

bool GetMD5SUM(const char* pFileName, unsigned char* pDigest)
{
    MD5_CTX mdContext;
    unsigned char data[1024];
    int bytes = 0;
    bool bSuccess = false;
    FILE* pFile = fopen(pFileName, "rb");

    if (pFile && pDigest) {
        MD5_Init(&mdContext);

        while ((bytes = fread (data, 1, 1024, pFile)) != 0) {
            MD5_Update(&mdContext, data, bytes);
        }

        MD5_Final(pDigest, &mdContext);
        bSuccess = true;
    }
    if (pFile) {
        fclose(pFile);
    }
    return bSuccess;
}

int AutoDownload(const char* pDumpFile)
{
    unsigned char hash[MD5_DIGEST_LENGTH];
    int i = 0;
    for (int index = 0; index < TEST_URL_COUNT; index++) {
        printf("\ngUrlList[%d]: [%s]\n", index, gUrlList[index]);
        if (GetMD5SUM("/var/www/html/Media/Big_Buck_Bunny.mp4", hash)) {
            for(i = 0; i < MD5_DIGEST_LENGTH; i++) {
                printf("%02x", hash[i]);
            }
        }
        break;
    }
    return 0;
}

//#define DOWNLOAD_AUDIO
//#define DOWNLOAD_VIDEO
//#define DOWNLOAD_WEBPAGE

int main() {

#ifdef DOWNLOAD_AUDIO
    const char* pAudioURL = "http://uyeshare.com/music/down/22440314/2068304/ZjQzOWkwRmlvbnFlYUd0dDNSeUVlNXd0UTcwQ3lEOEpyWmpSMk5FRzBuR1htUFNzWmc=/Cleopatra+Stratan+-+Zunea-Zunea.mp3";
    //DownloadPage("http://m1501.wapka-file.com//g03/music/22440314/2068304/15cf7nsY^po9LuL1cvteN^xPYT0kUu7aBTqL8ZwE9vnc3x6Exw/d6db7.mp3?md5=Qm95tfGKZVYF3sO3rPHZtw&expires=1483918968");
#endif
#ifdef DOWNLOAD_VIDEO
    //DownloadPage("http://www.mp3nobs.com/video/down/44780513/2473608/MDc1Ynl4UjFGRTR5MXRJakZIVFJZVHhDdVQ5TDJxS0xEM29kT2Z2R29iVFMrMlhDTHc=/Adele%E2%80%93Send_My_Love_%28Mp3Nobs.com%29.mp4");
    //DownloadPage("http://10.11.50.200/Media/SampleVideo_1280x720_30mb.mp4");
    //DownloadPage("http://10.11.50.200/Media/Cleopatra%20Stratan%20-%20Zunea-Zunea.mp3");
    DownloadPage("http://www.example.com/");
#endif
#ifdef DOWNLOAD_WEBPAGE
    DownloadPage("http://www.example.com/");
#endif
#ifdef NOVA
    DownloadPageWithNova("http://www.example.com/");
#endif
    AutoDownload("");
    return 0;
}
