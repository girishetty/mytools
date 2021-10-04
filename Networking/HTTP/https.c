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
#include "https.h"

#include <curl/curl.h>
#include <curl/multi.h>
#include <string.h>

//#define ALOGI printf
//#define ALOGE printf
#define ALOGI(...)
#define ALOGE(...)

HttpsCode HttpsLibInit() {
  return curl_global_init(CURL_GLOBAL_ALL);
}

void HttpsLibFini() {
  curl_global_cleanup();
}

//single
HttpsCode HttpsSingleSetURL(const HttpsSingle *single, const char * URL) {
  ALOGI("in function %s\n", __func__);
  HttpsCode ret = NOVA_NET_ERR_OK;
  if (single == NULL || URL == NULL) {
      return NOVA_NET_ERR_INVALID_POINTER;
  }
  //curl_easy_setopt((CURL *)single->single_handle, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_2);
  return curl_easy_setopt((CURL *)single->single_handle, CURLOPT_URL, URL);
}

HttpsCode HttpsSingleSetRequestMethod(const HttpsSingle *single, const HttpRequestType type){
  HttpsCode ret = NOVA_NET_ERR_OK;
  if (single == NULL) {
      return NOVA_NET_ERR_INVALID_POINTER;
  }
  ALOGI("in function %s\n", __func__);
  switch (type) {
    case HTTP_GET:
      ret = curl_easy_setopt((CURL *)single->single_handle, CURLOPT_HTTPGET, 1);
      break;
    case HTTP_HEAD:
      ret = curl_easy_setopt((CURL *)single->single_handle, CURLOPT_NOBODY, 1);
      break;
    default:
      ALOGI("HTTP request method not yet supported\n");
      ret = NOVA_NET_ERR_UNKNOWN_OPTION;
      break;
  }
  return ret;
}

HttpsCode HttpsSingleSetResponseCallback(const HttpsSingle *single,
                                         size_t (*func)(void *contents, size_t size, size_t nmemb, void *user),
                                          const void *ptr) {
  HttpsCode ret = NOVA_NET_ERR_OK;
  if (single == NULL) {
      return NOVA_NET_ERR_INVALID_POINTER;
  }
  if (func != NULL) {
    /* send all data to this function  */
    ret = curl_easy_setopt((CURL *)single->single_handle, CURLOPT_WRITEFUNCTION, func);
    if (ret != CURLE_OK)  return ret;
    ret = curl_easy_setopt((CURL *)single->single_handle, CURLOPT_WRITEDATA, ptr);
    if (ret != CURLE_OK)  return ret;
    ret = curl_easy_setopt((CURL *)single->single_handle, CURLOPT_HEADER, 1);
    ALOGI("in function %s callback\n", __func__);

  } else {
    /* pass FILE pointer */
    ret = curl_easy_setopt(single->single_handle, CURLOPT_WRITEDATA, (FILE *)ptr);
    ALOGI("in function %s FILE\n", __func__);
  }

  return ret;
}

HttpsCode HttpsSingleSetResponseBodyCallback(const HttpsSingle *single,
					 size_t (*func)(void *contents, size_t size,
							size_t nmemb, void *user),
					 const void *ptr) {
  HttpsCode ret = NOVA_NET_ERR_OK;
  if (single == NULL) {
      return NOVA_NET_ERR_INVALID_POINTER;
  }
  if (func != NULL) {
    /* send all data to this function  */
    ret = curl_easy_setopt((CURL *)single->single_handle, CURLOPT_WRITEFUNCTION, func);
    ret = curl_easy_setopt((CURL *)single->single_handle, CURLOPT_WRITEDATA, ptr);
    ALOGI("in function %s callback\n", __func__);
  } else {
    /* pass FILE pointer */
    ret = curl_easy_setopt(single->single_handle, CURLOPT_WRITEDATA, (FILE *)ptr);
    ALOGI("in function %s FILE\n", __func__);
  }

  return ret;
}

HttpsCode HttpsSingleSetResponseHeaderCallback(const HttpsSingle *single,
                                              size_t (*func)(char *buffer,   size_t size, size_t nitems, void *userdata),
                                              const void *userp) {
  HttpsCode ret = NOVA_NET_ERR_OK;
  if (single == NULL) {
      return NOVA_NET_ERR_INVALID_POINTER;
  }
  ret = curl_easy_setopt((CURL *)single->single_handle, CURLOPT_HEADERFUNCTION, func);
  if (ret != CURLE_OK)  return ret;
  ret = curl_easy_setopt((CURL *)single->single_handle, CURLOPT_HEADERDATA, userp);
  ALOGI("in function %s callback progress\n", __func__);
  return ret;
}

HttpsCode HttpsSingleSetFileDownUpLoadProgressCallback(const HttpsSingle *single,
                                       int (*func)(void *userp, int64_t dltotal, int64_t dlnow,
                                          int64_t ultotal, int64_t ulnow), const void *userp) {
  HttpsCode ret = NOVA_NET_ERR_OK;
  if (single == NULL) {
      return NOVA_NET_ERR_INVALID_POINTER;
  }
  ret = curl_easy_setopt((CURL *)single->single_handle, CURLOPT_XFERINFOFUNCTION, func);
  if (ret != CURLE_OK)  return ret;
  ret = curl_easy_setopt((CURL *)single->single_handle, CURLOPT_XFERINFODATA, userp);
  if (ret != CURLE_OK)  return ret;
  ret = curl_easy_setopt((CURL *)single->single_handle, CURLOPT_NOPROGRESS, 0L);
  ALOGI("in function %s callback header\n", __func__);
  return ret;
}

HttpsCode HttpsSinglePauseRequest(const HttpsSingle *single) {
  ALOGI("in function %s\n", __func__);
  return curl_easy_pause((CURL *)single->single_handle, CURLPAUSE_ALL);
}

HttpsCode HttpsSingleResumeRequest(const HttpsSingle *single) {
  ALOGI("in function %s\n", __func__);
  return curl_easy_pause((CURL *)single->single_handle, CURLPAUSE_CONT);
}

HttpsCode HttpsSingleCleanup(const HttpsSingle *single) {
  ALOGI("in function %s\n", __func__);
  if (single == NULL) {
      return NOVA_NET_ERR_INVALID_POINTER;
  }
  if (single->header_field) {
    curl_slist_free_all((struct curl_slist *)single->header_field);
  }
  curl_easy_cleanup(single->single_handle);
  return NOVA_NET_ERR_OK;
}

HttpsCode HttpsSingleSetRequestStatusCallback(HttpsSingle *single,
                                       void (*func)(HttpsSingleStatus *single_status, const void *userp),
                                       const void *userp) {
  HttpsCode ret = NOVA_NET_ERR_OK;
  if (single == NULL) {
      return NOVA_NET_ERR_INVALID_POINTER;
  }
  single->request_status_callback = func;
  single->status_callback_userp = userp;
  ALOGI("in function %s callback\n", __func__);
  return ret;
}

HttpsCode HttpsSingleSetDebugTraceCallback(HttpsSingle *single,
                                       void (*func)(void *handle, trace_info_type type, char *data, size_t size, void *userp),
                                       const void *userp) {
  HttpsCode ret = NOVA_NET_ERR_OK;
  if (single == NULL) {
      return NOVA_NET_ERR_INVALID_POINTER;
  }
  ret = curl_easy_setopt((CURL *)single->single_handle, CURLOPT_DEBUGFUNCTION, func);
  if (ret != CURLE_OK)  return ret;
  ret = curl_easy_setopt((CURL *)single->single_handle, CURLOPT_DEBUGDATA, userp);
  if (ret != CURLE_OK)  return ret;
  ret = curl_easy_setopt((CURL *)single->single_handle, CURLOPT_VERBOSE, 1L);
  ALOGI("in function %s callback\n", __func__);
  return ret;
}

//only supported in libcurl 7.50 and later
HttpsCode HttpsSingleGetHTTPVersion(const HttpsSingle *single, long *http_version) {
  HttpsCode ret = NOVA_NET_ERR_OK;
  if (single == NULL) {
      return NOVA_NET_ERR_INVALID_POINTER;
  }
  ret = curl_easy_getinfo((CURL *)single->single_handle, CURLOPT_HTTP_VERSION, http_version);
  return ret;
}

HttpsCode HttpsSingleGetResponseCode(const HttpsSingle *single, long *response_code ) {
  HttpsCode ret = NOVA_NET_ERR_OK;
  if (single == NULL) {
      return NOVA_NET_ERR_INVALID_POINTER;
  }
  ret = curl_easy_getinfo((CURL *)single->single_handle, CURLINFO_RESPONSE_CODE, response_code);
  return ret;;
}

HttpsCode HttpsSingleGetContentLength(const HttpsSingle *single, long long *content_length) {
  HttpsCode ret = NOVA_NET_ERR_OK;
  if (single == NULL || content_length == NULL) {
      return NOVA_NET_ERR_INVALID_POINTER;
  }
  double content_length_tmp;
  ret = curl_easy_getinfo((CURL *)single->single_handle, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &content_length_tmp);
  *content_length = (long long)content_length_tmp;
  return ret;
}

HttpsCode HttpsSingleGetLastEffectiveURL(const HttpsSingle *single, char **URL) {
  HttpsCode ret = NOVA_NET_ERR_OK;
  if (single == NULL) {
      return NOVA_NET_ERR_INVALID_POINTER;
  }
  ret = curl_easy_getinfo((CURL *)single->single_handle, CURLINFO_EFFECTIVE_URL, URL);
  return ret;
}

HttpsCode HttpsSingleGetContentType(const HttpsSingle *single, char **content_type) {
  HttpsCode ret = NOVA_NET_ERR_OK;
  if (single == NULL) {
      return NOVA_NET_ERR_INVALID_POINTER;
  }
  ret = curl_easy_getinfo((CURL *)single->single_handle, CURLINFO_CONTENT_TYPE, content_type);
  return ret;
}

HttpsCode HttpsSingleSetHeaderField(HttpsSingle *single, const char *field) {
  HttpsCode ret = NOVA_NET_ERR_OK;
  if (single == NULL || field == NULL) {
      return NOVA_NET_ERR_INVALID_POINTER;
  }

  single->header_field = (void *)curl_slist_append((struct curl_slist *)single->header_field, field);
  if (single->header_field == NULL) {
    return NOVA_NET_ERR_OUT_OF_MEMORY;
  }
  return ret;
}

HttpsCode HttpsSingleInit(HttpsSingle *single) {
  HttpsCode ret = NOVA_NET_ERR_OK;
  if (single == NULL) {
      return NOVA_NET_ERR_INVALID_POINTER;
  }
  single->single_handle = (HttpsSingleHandle *)curl_easy_init();
  single->SetURL = HttpsSingleSetURL;
  single->SetRequestMethod = HttpsSingleSetRequestMethod;
  single->SetResponseBodyCallback = HttpsSingleSetResponseBodyCallback;
  single->SetResponseHeaderCallback = HttpsSingleSetResponseHeaderCallback;
  single->SetFileDownUpLoadProgressCallback = HttpsSingleSetFileDownUpLoadProgressCallback;
  single->SetRequestStatusCallback = HttpsSingleSetRequestStatusCallback;
  single->SetDebugTraceCallback = HttpsSingleSetDebugTraceCallback;
  single->PauseRequest = HttpsSinglePauseRequest;
  single->ResumeRequest = HttpsSingleResumeRequest;
  single->Cleanup = HttpsSingleCleanup;
  //single->GetHTTPVersion = HttpsSingleGetHTTPVersion;
  single->GetResponseCode = HttpsSingleGetResponseCode;
  single->GetContentLength = HttpsSingleGetContentLength;
  single->SetHeaderField = HttpsSingleSetHeaderField;
  single->GetLastEffectiveURL = HttpsSingleGetLastEffectiveURL;
  single->GetContentType = HttpsSingleGetContentType;
  single->status_callback_userp = NULL;
  single->header_field = NULL;
  ALOGI("in function %s\n", __func__);
  return ret;
}

//multi
HttpsCode HttpsMultiAddSingle(HttpsMulti *multi, const HttpsSingle *single) {
  ALOGI("in function %s\n", __func__);
  HttpsCode ret = NOVA_NET_ERR_OK;
  int i;
  if (multi == NULL || single == NULL) {
      return NOVA_NET_ERR_INVALID_POINTER;
  }
  for (i = 0; i < MAX_SINGLES_IN_MULTI; i++) {
    if (multi->singles[i] == NULL) {
      multi->singles[i] = single;
      break;
    }
  }
  if (i == MAX_SINGLES_IN_MULTI) {
    ALOGI("in function %s excceed single limit \n", __func__);
    ret = NOVA_NET_ERR_OUT_OF_LIMIT;
    return ret;
  }
  // for later
  /* enable TCP keep-alive for this transfer */
  //curl_easy_setopt(curl, CURLOPT_TCP_KEEPALIVE, 1L);

  /* keep-alive idle time to 120 seconds */
  //curl_easy_setopt(curl, CURLOPT_TCP_KEEPIDLE, 120L);

  /* interval time between keep-alive probes: 60 seconds */
  //curl_easy_setopt(curl, CURLOPT_TCP_KEEPINTVL, 60L);
  if (single->header_field) {
    ret = curl_easy_setopt((CURL *)single->single_handle, CURLOPT_HTTPHEADER, (struct curl_slist *)single->header_field);
  }
  if (ret != CURLE_OK)   return ret;
  ret = curl_multi_add_handle((CURLM *)multi->multi_handle, (CURL *)(single->single_handle));

  return ret;
}

static HttpsSingle* HelperFindSingle(const HttpsMulti *multi, HttpsSingleHandle *single_handle) {
  ALOGI("in function %s\n", __func__);
  int i;
  if (single_handle == NULL) {
    return NULL;
  }
  for (i = 0; i < MAX_SINGLES_IN_MULTI; i++) {
    if (multi->singles[i] && multi->singles[i]->single_handle == single_handle) {
      return (HttpsSingle*)multi->singles[i];
    }
  }
  if (i == MAX_SINGLES_IN_MULTI) {
    ALOGI("in function %s excceed single limit \n", __func__);
  }
  return NULL;
}

static HttpsCode HelperRemoveSingle(HttpsMulti *multi, HttpsSingle *single) {
  ALOGI("in function %s\n", __func__);
  int i;
  HttpsCode ret = NOVA_NET_ERR_OK;
  if (single == NULL) {
    return NOVA_NET_ERR_GOT_NOTHING;
  }
  for (i = 0; i < MAX_SINGLES_IN_MULTI; i++) {
    if (multi->singles[i] == single) {
      ret = curl_multi_remove_handle((CURLM *)multi->multi_handle, (CURL *)(single->single_handle));
      multi->singles[i] = NULL;
      return ret;
    }
  }
  if (i == MAX_SINGLES_IN_MULTI) {
    ALOGI("in function %s excceed single limit \n", __func__);
    ret = NOVA_NET_ERR_OUT_OF_LIMIT;
  }
  return ret;
}

// NOTE HttpsMultiExecRequests and HttpsMultiWaitCompletion is combined to
// become a blocking call.
HttpsCode HttpsMultiExecRequests(HttpsMulti *multi) {
  int still_running = 0;
  int numfds = 0;
  HttpsCode ret = NOVA_NET_ERR_OK;
  struct CURLMsg *m;
  int msgq = 0;
  HttpsSingleStatus single_status;

  if (multi == NULL) {
      return NOVA_NET_ERR_INVALID_POINTER;
  }
  curl_multi_perform(multi->multi_handle, &still_running);
  do {
    ret = curl_multi_wait(multi->multi_handle, NULL, 0, 100, &numfds);
    if(ret != CURLM_OK) {
      ALOGE("error: curl_multi_wait() returned %d\n", ret);
      return ret;
    }

    curl_multi_perform(multi->multi_handle, &still_running);
    m = curl_multi_info_read(multi->multi_handle, &msgq);
    if(m && (m->msg == CURLMSG_DONE)) {
      HttpsSingle *single = HelperFindSingle(multi, m->easy_handle);
      if (single == NULL) {
        ALOGE("error: failed to find single %p %d\n", single, ret);
        return ret;
      }
      HttpsSingleStatus single_status;
      single_status.single = single;
      single_status.status = m->data.result;
      if (single->request_status_callback) {
        single->request_status_callback(&single_status, single->status_callback_userp);
      }
      //auto remove
      ret = HelperRemoveSingle(multi, single);
      if (ret != NOVA_NET_ERR_OK) {
        ALOGE("error: failed to auto remove single %p %d\n", single, ret);
      }
    }
  } while(still_running);

  //check single again, it may just happened
  do {
    m = curl_multi_info_read(multi->multi_handle, &msgq);
    if(m && (m->msg == CURLMSG_DONE)) {
      HttpsSingle *single = HelperFindSingle(multi, m->easy_handle);
      if (single == NULL) {
        ALOGE("error: failed to find single %p %d\n", single, ret);
        return ret;
      }
      HttpsSingleStatus single_status;
      single_status.single = single;
      single_status.status = m->data.result;
      if (single->request_status_callback) {
        single->request_status_callback(&single_status, single->status_callback_userp);
      }
      //auto remove
      ret = HelperRemoveSingle(multi, single);
      if (ret != NOVA_NET_ERR_OK) {
        ALOGE("error: failed to auto remove single %p %d\n", single, ret);
      }
    }
  } while (m);

  ALOGI("in function %s\n", __func__);
  return ret;
}

HttpsCode HttpsMultiWaitCompletion(const HttpsMulti *multi, int timeout_ms) {
  // obsolete, will remove. For now leave it for download manager
  return 0;
}

HttpsCode HttpsMultiCleanup(const HttpsMulti *multi) {
  ALOGI("in function %s\n", __func__);
  if (multi == NULL) {
      return NOVA_NET_ERR_INVALID_POINTER;
  }
  return (HttpsCode)curl_multi_cleanup(multi->multi_handle);
}

HttpsCode HttpsMultiInit(HttpsMulti *multi) {
  HttpsCode ret = NOVA_NET_ERR_OK;
  if (multi == NULL) {
      return NOVA_NET_ERR_INVALID_POINTER;
  }
  multi->AddSingle = HttpsMultiAddSingle;
  multi->Cleanup = HttpsMultiCleanup;
  multi->ExecRequests = HttpsMultiExecRequests;
  multi->WaitCompletion = HttpsMultiWaitCompletion;

  int i;
  for (i = 0; i < MAX_SINGLES_IN_MULTI; i++) {
    multi->singles[i] = NULL;
  }
  multi->multi_handle = (HttpsMultiHandle *)curl_multi_init();
  ALOGI("in function %s\n", __func__);
  return ret;
}

