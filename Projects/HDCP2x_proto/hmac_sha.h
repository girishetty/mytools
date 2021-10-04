#ifndef _HMAC_SHA_H_
#define _HMAC_SHA_H_

#include "ctypes.h"

typedef enum _SHA_ALGORITHM_TYPE {
   ALG_SHA1,
   ALG_SHA224,
   ALG_SHA256,
   ALG_SHA384,
   ALG_SHA512,
}SHA_ALGORITHM_TYPE;

/*
 * Function:
 *  HMAC_SHA: Performs HMAC_SHA algorithm
 *
 * Arguments:
 *  text:      pointer to data stream
 *  text_len:  length of data stream
 *  key:       pointer to authentication key
 *  key_len:   length of authentication key
 *  digest:    caller digest to be filled in
 *  mode:      mode of algorithm (SHA_ALGORITHM_TYPE)
 */
STATUS HMAC_SHA(const UINT8* text, int text_len, const UINT8* key, int key_len, UINT8* digest, INT32 mode);

/*
 *  text:      pointer to data stream
 *  text_len:  length of data stream
 *  key:       pointer to authentication key
 *  key_len:   length of authentication key
 *  digest:    caller digest to be filled in
 */
void hmac_sha256(const UINT8* text, INT32 text_len, const UINT8* key, INT32 key_len, void* digest);

#endif

