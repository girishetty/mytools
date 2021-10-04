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

/** @file MediaHTTPConnection.h
*   @brief MediaHTTPConnection Class
*/

#pragma once

#include <https.h>
#include <string>
#include <utils/Mutex.h>
#include <utils/RefBase.h>
#include <utils/String8.h>
#include <utils/KeyedVector.h>

namespace MagicLeap {
namespace Media {

typedef android::KeyedVector<android::String8, android::String8> HeaderVector;

/*
 * @class MediaHTTPConnection
 * @brief MediaHTTPConnection exposes HTTP connection functionalities for Multimedia subsystem
 */
struct MediaHTTPConnection : public android::RefBase {

    /**
     * @brief Constructor
     */
    MediaHTTPConnection();

    /**
     * @brief Destructor
     */
    virtual ~MediaHTTPConnection();

    /**
     * @brief Make a HTTP Connection to given URI
     *
     * @param[in] uri URI of the http request
     * @param[in] headers Vector of pairs for header information
     * @return true on success, false otherwise
     */
    bool connect(const char* uri, const HeaderVector* headers);

    /**
     * @brief Disconnect from the current HTTP Connection
     */
    void disconnect();

    /**
     * @brief Read desired size of data into given buffer
     *
     * @param[in] offset Offset within the media
     * @param[out] data Buffer to copy the media into
     * @param[in] size No of bytes of data to read
     * @return no of bytes read on success, -1 otherwise
     */
    ssize_t readAt(off64_t offset, void* data, size_t size);

    /**
     * @brief Gets the size of the media content
     *
     * @return size of the media contents as no of bytes
     */
    off64_t getSize();

    /**
     * @brief Gets the Mime-type of the media content
     * @param[out] mimeType to copy the mime-type
     *
     * @return OK on success, error code otherwise
     */
    android::status_t getMIMEType(android::String8* mimeType);

    /**
     * @brief Gets the Effective URI of the media content
     * @param[out] uri to copy the URI
     *
     * @return OK on success, error code otherwise
     */
    android::status_t getUri(android::String8* uri);

private:
    enum InfoType {
        ECONTENTLENGTH = 0,
        ECONTENTTYPE,
        EURI,
    };

    enum ConnectionState {
        ECONNECTIONFAILED = -1, //Error case
        EDISCONNECTED = 0,      //HTTPS disconnected - default case
        ECONNECTED,             //HTTPS connection succesful
    };

    MediaHTTPConnection(const MediaHTTPConnection& );
    MediaHTTPConnection operator=(const MediaHTTPConnection& );

    void disconnect_l();

    void closeConnection();

    void copyHttpsHeaders(const HeaderVector* headers);

    HttpsCode setHttpsHeaders(off64_t aRange, ssize_t aSize);

    bool makeHTTPSRequest(off64_t aRange = 0, ssize_t aSize = 0);

    void getContentLengthFromRange();

    android::status_t getInfo(InfoType type, std::string* info);

    ssize_t copyData(off64_t uOffset, void* pBuffer, size_t uSize) const;

    //Callback for Reponse Status
    void processResponseStatus(HttpsSingleStatus* pStatus);

    //Callback for HTTP Header
    size_t processHttpsHeader(const char* pHeader, size_t uSize, size_t uCount);

    //Callback for HTTP Body
    size_t processHttpsBody(const char* pBody, size_t uSize, size_t uCount);

    friend void ProcessHttpsResponseStatusCallback(HttpsSingleStatus* pStatus, void* p);
    friend size_t ProcessHttpsHeaderCallback(char* pHeader, size_t uSize, size_t uCount, void* p);
    friend size_t ProcessHttpsBodyCallback(void* pBody, size_t uSize, size_t uCount, void* p);

    //private member variables for handling HTTPS connection
    ConnectionState  mConnectionState;
    HttpsMulti       mHttpsMulti;
    HttpsSingle*     mHttpsSingle;
    std::string      mURL;
    std::string      mContentType;
    std::string      mContentRange;
    long             mResponseCode;
    long long        mContentTotalLength;
    long long        mCurrentOffset;
    size_t           mCurrentSize;
    uint8_t*         mMemory;
    bool             mFirstConnectionDone;
    bool             mTriedWithCookie;
    android::Mutex   mLock;
    HeaderVector     mHeaders;
};

}  // namespace Media
}  // namespace MagicLeap
