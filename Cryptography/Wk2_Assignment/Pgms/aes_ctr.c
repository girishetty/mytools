#include <stdlib.h>
#include <stdio.h>
#include <openssl/aes.h>
#include "common_def.h"

void aes_ctr_encrypt_(const UINT8* input, UINT32 inputLen, const UINT8* key, UINT8* iv, UINT8* output);
void aes_ctr_decrypt_(const UINT8* input, UINT32 inputLen, const UINT8* key, UINT8* iv, UINT8* output);


int aes_ctr(const UINT8* input, UINT32 inputLen, const UINT8* key, UINT8* iv, UINT8* output, int mode) {
    int res = SUCCESS;
    if(mode == AES_ENCRYPT) {
        aes_ctr_encrypt_(input, inputLen, key, iv, output);
    } else if (mode == AES_DECRYPT) {
        aes_ctr_decrypt_(input, inputLen, key, iv, output);
    } else {
        res = FAILURE;
    }

    return res;
}

#ifndef SSL_ENCRYPTION
/* The input encrypted as though 128bit counter mode is being
 * used.  The extra state information to record how much of the
 * 128bit block we have used is contained in *num, and the
 * encrypted counter is kept in ecount_buf.  Both *num and
 * ecount_buf must be initialised with zeros before the first
 * call to AES_ctr128_encrypt().
 */
void AES_ctr128_encrypt_(const UINT8* in, UINT8* out,
                         UINT32 length, const AES_KEY* key,
                         UINT8 counter[AES_BLOCK_SIZE],
                         UINT8 ecount_buf[AES_BLOCK_SIZE],
                         UINT32* num)
{
    UINT32 n = 0;

    if (in && out && key && counter && num &&
        *num < AES_BLOCK_SIZE) {

        n = *num;
        while (length--) {
            if (n == 0) {
                AES_encrypt(counter, ecount_buf, key);
                AES_ctr128_inc(counter);
            }
            *(out++) = *(in++) ^ ecount_buf[n];
            n = (n + 1) % AES_BLOCK_SIZE;
        }
        *num = n;
    }
}
#endif

void aes_ctr_encrypt_(const UINT8* input, UINT32 inputLen, const UINT8* key, UINT8* iv, UINT8* output) {

    int keylength = 128;  //16 byte key (128 bits)
    int iv_len = 16;      //16 byte iencryption IV, randomly selected

    //prefix the 16-byte encryption IV to ciphertext and initialize CRT IV
    memcpy(output, iv, iv_len);

    // AES CTR requires 'num' and 'ecount' set to zero on the first call
    int num = 0;
    UINT8 eCount[16] = { 0 };
    memset(eCount, 0, 16);

    //encrypt
    AES_KEY aesKey;
    AES_set_encrypt_key(key, keylength, &aesKey);

#ifdef SSL_ENCRYPTION
    AES_ctr128_encrypt(input, output + iv_len, inputLen, &aesKey, iv, eCount, &num);
#else
    AES_ctr128_encrypt_(input, output + iv_len, inputLen, &aesKey, iv, eCount, &num);
#endif
}

void aes_ctr_decrypt_(const UINT8* input, UINT32 inputLen, const UINT8* key, UINT8* iv, UINT8* output) {

    int keylength = 128;  //16 byte key (128 bits)
    int iv_len = 16;      //16 byte iencryption IV, randomly selected

    // AES CTR requires 'num' and 'ecount' set to zero on the first call
    int num = 0;
    UINT8 eCount[16] = { 0 };
    memset(eCount, 0, 16);

    //decrypt - AES CTR uses the same function for encryption/decryption
    AES_KEY aesKey;
    AES_set_encrypt_key(key, keylength, &aesKey);

#ifdef SSL_ENCRYPTION
    AES_ctr128_encrypt(input, output, inputLen, &aesKey, iv, eCount, &num);
#else
    AES_ctr128_encrypt_(input, output, inputLen, &aesKey, iv, eCount, &num);
#endif
}
