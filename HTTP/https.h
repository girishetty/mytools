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

// This file defines Nova HTTPS APIs and necessary data structures
// It tries to simulate objected oriented style with C, function pointers
// are used to simulate member methods \n

// The APIs are targeted for HTTP2 usage, mainly for multiple HTTP requests
// in multiplexing fashion. User provides callback function
// to process incoming or outgoing data as soon as data is available
#ifdef __cplusplus
extern "C" {
#endif

#pragma once

#include "https_def.h"
#include <stddef.h>
#include <stdint.h>

typedef int HttpsCode;
typedef struct _HttpsSingle HttpsSingle;
typedef struct _HttpsMulti HttpsMulti;
typedef void HttpsSingleHandle;
typedef void HttpsMultiHandle;

/// HTTP status callback data structure, containing ponter to single and status code
typedef struct _HttpsSingleStatus {
  HttpsSingle*  single;
  HttpsCode     status;
} HttpsSingleStatus;

typedef struct _HttpsSingle {
  HttpsSingleHandle*         single_handle;
  const void*   status_callback_userp;
  void          (*request_status_callback)(HttpsSingleStatus *single_status, const void *userp);
  void*         header_field;

  /// function to set URL string for single HTTP request \n
  /// IN:       single \n
  /// IN:       URL \n
  /// Return:   Ok or error code \n
  HttpsCode     (*SetURL)(const HttpsSingle *single, const char * URL);

  /// function to set request type for single HTTP request \n
  /// IN:       single \n
  /// IN:       type, of GET, POST, PUT \n
  /// Return:   Ok or error code \n
  HttpsCode     (*SetRequestMethod)(const HttpsSingle *single, const HttpRequestType type);

  /// function to set user's callback function to process HTTP response body excluding HTTP header.
  /// if user want to simply save repsonse body without header into a file, then supplying a NULL
  /// pointer for callback function and a FILE pointer for userp\n
  /// IN:       single
  /// IN:       callback, the function format must match\n
  /// IN:       usrep, user provided pointer to pass to callback function.
  ///           for example, the pointer of single can be passed so that
  ///           the callback function knows the data is for which request\n
  /// IN:       usrep, user provided pointer to pass to callback function.
  ///           for example, the pointer of single can be passed so that
  ///           the callback function knows the data is for which request\n
  /// Return:   Ok or error code\n
  HttpsCode     (*SetResponseBodyCallback)(const HttpsSingle *single,
                                            size_t (*func)(void *contents, size_t size, size_t nmemb, void *userp),
                                            const void *userp);

  /// function to set user's callback function to process HTTP response containing only HTTP header\n
  /// IN:       single
  /// IN:       callback, the function format must match
  /// IN:       usrep, user provided pointer to pass to callback function.
  ///           for example, the pointer of single can be passed so that
  ///           the callback function knows the data is for which request\n
  /// Return:   Ok or error code\n
  HttpsCode     (*SetResponseHeaderCallback)(const HttpsSingle *single,
                                        size_t (*func)(char *buffer,   size_t size, size_t nitems,   void *userp),
                                        const void *userp);

  /// when doing file download (HTTP_GET) or upload (HTTP_PUT), user has an option
  /// not to provide a callback function in SetResponseCallback(). Instead, a FILE
  /// pointer may be sufficient. This function allows to calculate file transfer progress\n
  /// IN:        single\n
  /// IN:        callback, the function format must match\n
  /// IN:        usrep, user provided pointer to pass to callback function.
  ///            for example, the pointer of single can be passed so that
  ///            the callback function knows the data is for which request\n
  /// Return:    Ok or error code
  HttpsCode     (*SetFileDownUpLoadProgressCallback)(const HttpsSingle *single,
                                       int (*func)(void *userp, int64_t dltotal, int64_t dlnow,
                                          int64_t ultotal, int64_t ulnow), const void *userp);

  /// function to set user's callback function to process single HTTP request status.
  /// for example, if request fails in TCP level, header callback will not be triggered
  /// but this callback will be called no matter what\n
  /// IN:        single\n
  /// IN:        callback, the function format must match\n
  /// IN:        usrep, user provided pointer to pass to callback function.
  ///            for example, the pointer of single can be passed so that
  ///            the callback function knows the data is for which request\n
  /// Return:    Ok or error code\n
  HttpsCode      (*SetRequestStatusCallback)(HttpsSingle *single,
                                       void (*func)(HttpsSingleStatus *single_status, const void *userp),
                                       const void *userp);

  /// function to set user's callback function to process debug trace.
  /// The debug trace is like wireshark in HTTP level.
  /// User can choose to save trace in a file, note stderr is a valid FILE pointer
  /// which prints to console\n
  /// IN:        single\n
  /// IN:        callback, the function format must match\n
  /// IN:        usrep, user provided pointer to pass to callback function.
  ///            The option of saving trace to file, and trace level\n
  /// Return:    Ok or error code\n
  HttpsCode      (*SetDebugTraceCallback)(HttpsSingle *single,
                                       void (*func)(void *handle, trace_info_type type, char *data, size_t size, void *userp),
                                       const void *userp);

  /// function to customize HTTP header\n
  /// IN:       single\n
  /// IN:       header field (the whole single line)\n
  /// Return:   Ok or error code\n
  HttpsCode     (*SetHeaderField)(HttpsSingle *single, const char *field);

  /// function to pause HTTP request\n
  /// IN:       single\n
  /// Return:   Ok or error code\n
  HttpsCode     (*PauseRequest)(const HttpsSingle *single);

  /// function to resume HTTP request\n
  /// IN:       single\n
  /// Return:   Ok or error code\n
  HttpsCode     (*ResumeRequest)(const HttpsSingle *single);

  /// function to cancel HTTP request\n
  /// IN:        single\n
  /// Return:    Ok or error code\n
  HttpsCode     (*CancelRequest)(const HttpsSingle *single);

  /// function to cleanup single instance when it is no longer needed\n
  /// IN:       single\n
  HttpsCode     (*Cleanup)(const HttpsSingle *single);

  // all HTTP header info can be parsed by user callback above as soon as it is available
  // following API is nice to have, only available after the request is fully done
  /// function to get HTTP response code\n
  /// IN:        single\n
  /// IN OUT:    response_code\n
  /// Return:    Ok or error code\n
  HttpsCode      (*GetResponseCode)(const HttpsSingle *single, long *response_code);

  /// function to get HTTP response content length\n
  /// IN:        single\n
  /// IN OUT:    content length\n
  /// Return:    Ok or error code\n
  HttpsCode      (*GetContentLength)(const HttpsSingle *single, long long *content_length);

  /// function to get last effective URL if redirected\n
  /// IN:        single\n
  /// IN OUT:    URL double pointer. Do NOT free by user\n
  /// Return:    Ok or error code\n
  HttpsCode      (*GetLastEffectiveURL)(const HttpsSingle *single, char **URL);

  /// function to get content type\n
  /// IN:        single\n
  /// IN OUT:    content type double pointer. Do NOT free by user\n
  /// Return:    Ok or error code\n
  HttpsCode      (*GetContentType)(const HttpsSingle *single, char **content_type);

  // TODO for API backward compatibility
  HttpsCode     (*reserved[32])();
} HttpsSingle;

// for multiple HTTP requests, benefit from HTTP2 multiplexing
typedef struct _HttpsMulti {
  HttpsMultiHandle*         multi_handle;
  const HttpsSingle*    singles[MAX_SINGLES_IN_MULTI];

  /// function to add a single instance to multi. Multiple single instances\n
  /// can be added before calling ExecRequests()\n
  /// IN:       multi\n
  /// IN:       single\n
  /// Return:   Ok or error code\n
  HttpsCode     (*AddSingle)(HttpsMulti *multi, const HttpsSingle *single);

  /// function to star performing requests with all added singles, blocking call.
  /// it gives chance for user callback functions to be called\n
  /// IN:       multi\n
  /// Return:   All done, or fatal error\n
  HttpsCode     (*ExecRequests)(HttpsMulti *multi);

  // this function is OBSOLETE, will remove soon
  HttpsCode     (*WaitCompletion)(const HttpsMulti *multi, const int timeout_ms);

  /// function to cleanup multi instance when it is no longer needed\n
  /// IN:       multi\n
  /// Return:   Ok or error code\n
  HttpsCode     (*Cleanup)(const HttpsMulti *multi);

  // TODO for API backward compatibility
  HttpsCode     (*reserved[32])();
} HttpsMulti;

// this is called only once by the application before using nova HTTP API
HttpsCode HttpsLibInit();

/// this is called ony once for the application before exiting
void HttpsLibFini();

/// function to initialize a single instance, like constructor of C++\n
/// IN OUT:     single
HttpsCode HttpsSingleInit(HttpsSingle *single);

/// function to initialize the multi instance, like constructor of C++\n
/// IN OUT: multi
HttpsCode HttpsMultiInit(HttpsMulti *multi);

#ifdef __cplusplus
}
#endif
