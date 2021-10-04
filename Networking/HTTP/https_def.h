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

#pragma once

#define MAX_SINGLES_IN_MULTI 128

typedef enum _HttpRequestType {
  HTTP_GET,
  HTTP_POST,
  HTTP_PUT,
  HTTP_HEAD,
} HttpRequestType;

enum {
  HTTP_VERSION_1_0 = 1,  /* HTTP 1.0 in the request */
  HTTP_VERSION_1_1,  /* HTTP 1.1 in the request */
  HTTP_VERSION_2_0,  /* HTTP 2.0 in the request */
};

//TODO: consolidate error code
enum {
  NOVA_NET_ERR_OK = 0,
  NOVA_NET_ERR_UNSUPPORTED_PROTOCOL,    /* 1 */
  NOVA_NET_ERR_FAILED_INIT,             /* 2 */
  NOVA_NET_ERR_URL_MALFORMAT,           /* 3 */
  NOVA_NET_ERR_NOT_BUILT_IN,            /* 4 - [was obsoleted in August 2007 for
                                    7.17.0, reused in April 2011 for 7.21.5] */
  NOVA_NET_ERR_COULDNT_RESOLVE_PROXY,   /* 5 */
  NOVA_NET_ERR_COULDNT_RESOLVE_HOST,    /* 6 */
  NOVA_NET_ERR_COULDNT_CONNECT,         /* 7 */
  NOVA_NET_ERR_FTP_WEIRD_SERVER_REPLY,  /* 8 */
  NOVA_NET_ERR_REMOTE_ACCESS_DENIED = 9,    /* 9 a service was denied by the server
                                    due to lack of access - when login fails
                                    this is not returned. */
  NOVA_NET_ERR_FTP_ACCEPT_FAILED,       /* 10 - [was obsoleted in April 2006 for
                                    7.15.4, reused in Dec 2011 for 7.24.0]*/
  NOVA_NET_ERR_FTP_WEIRD_PASS_REPLY,    /* 11 */
  NOVA_NET_ERR_FTP_ACCEPT_TIMEOUT,      /* 12 - timeout occurred accepting server
                                    [was obsoleted in August 2007 for 7.17.0,
                                    reused in Dec 2011 for 7.24.0]*/
  NOVA_NET_ERR_FTP_WEIRD_PASV_REPLY,    /* 13 */
  NOVA_NET_ERR_FTP_WEIRD_227_FORMAT,    /* 14 */
  NOVA_NET_ERR_FTP_CANT_GET_HOST,       /* 15 */
  NOVA_NET_ERR_HTTP2 = 16,                   /* 16 - A problem in the http2 framing layer.
                                    [was obsoleted in August 2007 for 7.17.0,
                                    reused in July 2014 for 7.38.0] */
 // NOVA_NET_ERR_FTP_COULDNT_SET_TYPE,    /* 17 */
  NOVA_NET_ERR_PARTIAL_FILE,            /* 18 */
  NOVA_NET_ERR_FTP_COULDNT_RETR_FILE,   /* 19 */
  NOVA_NET_ERR_OBSOLETE20,              /* 20 - NOT USED */
  NOVA_NET_ERR_QUOTE_ERROR,             /* 21 - quote command failure */
  NOVA_NET_ERR_HTTP_RETURNED_ERROR,     /* 22 */
  NOVA_NET_ERR_WRITE_ERROR,             /* 23 */
  NOVA_NET_ERR_OBSOLETE24,              /* 24 - NOT USED */
  NOVA_NET_ERR_UPLOAD_FAILED,           /* 25 - failed upload "command" */
  NOVA_NET_ERR_READ_ERROR,              /* 26 - couldn't open/read from file */
  NOVA_NET_ERR_OUT_OF_MEMORY,           /* 27 */
  /* Note: NOVA_NET_ERR_OUT_OF_MEMORY may sometimes indicate a conversion error
           instead of a memory allocation error if CURL_DOES_CONVERSIONS
           is defined
  */
  NOVA_NET_ERR_OPERATION_TIMEDOUT,      /* 28 - the timeout time was reached */
  NOVA_NET_ERR_OBSOLETE29,              /* 29 - NOT USED */
  NOVA_NET_ERR_FTP_PORT_FAILED,         /* 30 - FTP PORT operation failed */
  NOVA_NET_ERR_FTP_COULDNT_USE_REST,    /* 31 - the REST command failed */
  NOVA_NET_ERR_OBSOLETE32,              /* 32 - NOT USED */
  NOVA_NET_ERR_RANGE_ERROR,             /* 33 - RANGE "command" didn't work */
  NOVA_NET_ERR_HTTP_POST_ERROR,         /* 34 */
  NOVA_NET_ERR_SSL_CONNECT_ERROR,       /* 35 - wrong when connecting with SSL */
  NOVA_NET_ERR_BAD_DOWNLOAD_RESUME,     /* 36 - couldn't resume download */
  NOVA_NET_ERR_FILE_COULDNT_READ_FILE,  /* 37 */
  NOVA_NET_ERR_LDAP_CANNOT_BIND,        /* 38 */
  NOVA_NET_ERR_LDAP_SEARCH_FAILED,      /* 39 */
  NOVA_NET_ERR_OBSOLETE40,              /* 40 - NOT USED */
  NOVA_NET_ERR_FUNCTION_NOT_FOUND,      /* 41 */
  NOVA_NET_ERR_ABORTED_BY_CALLBACK,     /* 42 */
  NOVA_NET_ERR_BAD_FUNCTION_ARGUMENT,   /* 43 */
  NOVA_NET_ERR_OBSOLETE44,              /* 44 - NOT USED */
  NOVA_NET_ERR_INTERFACE_FAILED,        /* 45 - CURLOPT_INTERFACE failed */
  NOVA_NET_ERR_OBSOLETE46,              /* 46 - NOT USED */
  NOVA_NET_ERR_TOO_MANY_REDIRECTS ,     /* 47 - catch endless re-direct loops */
  NOVA_NET_ERR_UNKNOWN_OPTION,          /* 48 - User specified an unknown option */
  NOVA_NET_ERR_TELNET_OPTION_SYNTAX ,   /* 49 - Malformed telnet option */
  NOVA_NET_ERR_OBSOLETE50,              /* 50 - NOT USED */
  NOVA_NET_ERR_PEER_FAILED_VERIFICATION, /* 51 - peer's certificate or fingerprint
                                     wasn't verified fine */
  NOVA_NET_ERR_GOT_NOTHING,             /* 52 - when this is a specific error */
  NOVA_NET_ERR_SSL_ENGINE_NOTFOUND,     /* 53 - SSL crypto engine not found */
  NOVA_NET_ERR_SSL_ENGINE_SETFAILED,    /* 54 - can not set SSL crypto engine as
                                    default */
  NOVA_NET_ERR_SEND_ERROR,              /* 55 - failed sending network data */
  NOVA_NET_ERR_RECV_ERROR,              /* 56 - failure in receiving network data */
  NOVA_NET_ERR_OBSOLETE57,              /* 57 - NOT IN USE */
  NOVA_NET_ERR_SSL_CERTPROBLEM,         /* 58 - problem with the local certificate */
  NOVA_NET_ERR_SSL_CIPHER,              /* 59 - couldn't use specified cipher */
  NOVA_NET_ERR_SSL_CACERT,              /* 60 - problem with the CA cert (path?) */
  NOVA_NET_ERR_BAD_CONTENT_ENCODING,    /* 61 - Unrecognized/bad encoding */
  NOVA_NET_ERR_LDAP_INVALID_URL,        /* 62 - Invalid LDAP URL */
  NOVA_NET_ERR_FILESIZE_EXCEEDED,       /* 63 - Maximum file size exceeded */
  NOVA_NET_ERR_USE_SSL_FAILED,          /* 64 - Requested FTP SSL level failed */
  NOVA_NET_ERR_SEND_FAIL_REWIND,        /* 65 - Sending the data requires a rewind
                                    that failed */
  NOVA_NET_ERR_SSL_ENGINE_INITFAILED,   /* 66 - failed to initialise ENGINE */
  NOVA_NET_ERR_LOGIN_DENIED,            /* 67 - user, password or similar was not
                                    accepted and we failed to login */
  NOVA_NET_ERR_TFTP_NOTFOUND,           /* 68 - file not found on server */
  NOVA_NET_ERR_TFTP_PERM,               /* 69 - permission problem on server */
  NOVA_NET_ERR_REMOTE_DISK_FULL,        /* 70 - out of disk space on server */
  NOVA_NET_ERR_TFTP_ILLEGAL,            /* 71 - Illegal TFTP operation */
  NOVA_NET_ERR_TFTP_UNKNOWNID,          /* 72 - Unknown transfer ID */
  NOVA_NET_ERR_REMOTE_FILE_EXISTS,      /* 73 - File already exists */
  NOVA_NET_ERR_TFTP_NOSUCHUSER,         /* 74 - No such user */
  NOVA_NET_ERR_CONV_FAILED,             /* 75 - conversion failed */
  NOVA_NET_ERR_CONV_REQD,               /* 76 - caller must register conversion
                                    callbacks using curl_easy_setopt options
                                    CURLOPT_CONV_FROM_NETWORK_FUNCTION,
                                    CURLOPT_CONV_TO_NETWORK_FUNCTION, and
                                    CURLOPT_CONV_FROM_UTF8_FUNCTION */
  NOVA_NET_ERR_SSL_CACERT_BADFILE,      /* 77 - could not load CACERT file, missing
                                    or wrong format */
  NOVA_NET_ERR_REMOTE_FILE_NOT_FOUND,   /* 78 - remote file not found */
  NOVA_NET_ERR_SSH,                     /* 79 - error from the SSH layer, somewhat
                                    generic so the error message will be of
                                    interest when this has happened */

  NOVA_NET_ERR_SSL_SHUTDOWN_FAILED,     /* 80 - Failed to shut down the SSL
                                    connection */
  NOVA_NET_ERR_AGAIN,                   /* 81 - socket is not ready for send/recv,
                                    wait till it's ready and try again (Added
                                    in 7.18.2) */
  NOVA_NET_ERR_SSL_CRL_BADFILE,         /* 82 - could not load CRL file, missing or
                                    wrong format (Added in 7.19.0) */
  NOVA_NET_ERR_SSL_ISSUER_ERROR,        /* 83 - Issuer check failed.  (Added in
                                    7.19.0) */
  NOVA_NET_ERR_FTP_PRET_FAILED,         /* 84 - a PRET command failed */
  NOVA_NET_ERR_RTSP_CSEQ_ERROR,         /* 85 - mismatch of RTSP CSeq numbers */
  NOVA_NET_ERR_RTSP_SESSION_ERROR,      /* 86 - mismatch of RTSP Session Ids */
  NOVA_NET_ERR_FTP_BAD_FILE_LIST,       /* 87 - unable to parse FTP file list */
  NOVA_NET_ERR_CHUNK_FAILED,            /* 88 - chunk callback reported error */
  NOVA_NET_ERR_NO_CONNECTION_AVAILABLE, /* 89 - No connection available, the
                                    session will be queued */
  NOVA_NET_ERR_SSL_PINNEDPUBKEYNOTMATCH, /* 90 - specified pinned public key did not
                                     match */
  NOVA_NET_ERR_SSL_INVALIDCERTSTATUS,   /* 91 - invalid certificate status */

  NOVA_NET_ERR_INVALID_POINTER, /* 92 */
  NOVA_NET_ERR_OUT_OF_LIMIT, /* 93 */
  NOVA_NET_ERR_LAST /* never use! */
};

//for trace debugging
typedef enum {
  TRACE_INFO_TEXT = 0,
  TRACE_INFO_HEADER_IN,    /* 1 */
  TRACE_INFO_HEADER_OUT,   /* 2 */
  TRACE_INFO_DATA_IN,      /* 3 */
  TRACE_INFO_DATA_OUT,     /* 4 */
  TRACE_INFO_SSL_DATA_IN,  /* 5 */
  TRACE_INFO_SSL_DATA_OUT, /* 6 */
  TRACE_INFO_END
} trace_info_type;

typedef enum {
  TRACE_INFO_MASK_TEXT = 1,
  TRACE_INFO_MASK_HEADER_IN = 2,
  TRACE_INFO_MASK_HEADER_OUT = 4,
  TRACE_INFO_MASK_DATA_IN = 8,
  TRACE_INFO_MASK_DATA_OUT = 16,
  TRACE_INFO_MASK_SSL_DATA_IN = 32,
  TRACE_INFO_MASK_SSL_DATA_OUT = 64,
} trace_info_mask;