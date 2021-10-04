#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <openssl/aes.h>
#include "ctypes.h"
#include "aes_algo.h"

//#define SSL_ENCRYPTION

void aes_cbc_encrypt_(const UINT8* input, UINT32 inputLen, const UINT8* key, UINT8* iv, UINT8* output);
void aes_cbc_decrypt_(const UINT8* input, UINT32 inputLen, const UINT8* key, UINT8* iv, UINT8* output);

int aes_cbc(const UINT8* input, UINT32 inputLen, const UINT8* key, UINT8* iv, UINT8* output, int mode) {
    int res = STATUS_SUCCESS;
    if(mode == AES_ENCRYPT) {
        aes_cbc_encrypt_(input, inputLen, key, iv, output);
    } else if (mode == AES_DECRYPT) {
        aes_cbc_decrypt_(input, inputLen, key, iv, output);
    } else {
        res = STATUS_FAILURE;
    }

    return res;
}

void aes_cbc_encrypt_(const UINT8* input, UINT32 inputLen, const UINT8* key, UINT8* iv, UINT8* output) {
    AES_KEY enc_key;
    AES_set_encrypt_key(key, 128, &enc_key);
    AES_cbc_encrypt(input, output, inputLen, &enc_key, iv, AES_ENCRYPT);
}

void aes_cbc_decrypt_(const UINT8* input, UINT32 inputLen, const UINT8* key, UINT8* iv, UINT8* output) {
    AES_KEY dec_key;
    AES_set_decrypt_key(key, 128, &dec_key);
    AES_cbc_encrypt(input, output, inputLen, &dec_key, iv, AES_DECRYPT);
}

/*
 * AES - CTR
 */

int aes_ctr(const UINT8* input, UINT32 inputLen, const UINT8* key, UINT8* iv, UINT8* output, int mode) {
    int res = STATUS_SUCCESS;
    // AES CTR requires 'num' and 'ecount' set to zero on the first call
    int num = 0;
    UINT8 eCount[16] = { 0 };
    memset(eCount, 0, 16);

    if(mode == AES_ENCRYPT || mode == AES_DECRYPT) {
        AES_KEY aesKey;
        AES_set_encrypt_key(key, 128, &aesKey);

        AES_ctr128_encrypt(input, output, inputLen, &aesKey, iv, eCount, &num);
    } else {
        res = STATUS_FAILURE;
    }

    return res;
}

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
STATUS aes_process(UNSG8* in, UNSG8* out, UNSG8* iv, UNSG8* key, SIGN32 key_size, SIGN32 bytes, UNSG32 op, UNSG32 mode)
{
    STATUS ret = STATUS_SUCCESS;
    UINT8 iv_vector[16] = { 0x00 };
    if (iv == NULL) {
        memset(iv_vector, 0x00, 16);
        iv = iv_vector;
    }
    if (mode == AES_CBC) {
        ret = aes_cbc(in, bytes, key, iv, out, op);
    } else if (mode == AES_CTR) {
        ret = aes_ctr(in, bytes, key, iv, out, op);
    }

    return ret;
}

