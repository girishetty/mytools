#ifndef _AES_ALGO_H_
#define _AES_ALGO_H_

#include "ctypes.h"

#define AES_DECRYPT 0
#define AES_ENCRYPT 1

#define AES_ECB 0
#define AES_CBC 1
#define AES_CTR 2

/*
 * Encrypt/Decrypt data with AES algorithm
 *
 * Arguments:
 * - in       (IN) -  pointer to input data
 * - out      (IN) -  pointer to output data
 * - iv       (IN) -  pointer to intial vector
 * - key      (IN) -  pointer to key data
 * - key_size (IN) -  size of key data in bit
 * - bytes    (IN) -  size of input data in byte
 * - op       (IN) -  encrypt or decrypt flag
 * - mode     (IN) -  AES algorithm
 * Return:
 *         STATUS_SUCCESS or one of the BCM error code
 */
STATUS aes_process(UNSG8* in, UNSG8* out, UNSG8* iv, UNSG8* key, SIGN32 key_size, SIGN32 bytes, UNSG32 op, UNSG32 mode);

#endif

