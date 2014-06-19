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

    AES_ctr128_encrypt(input, output + iv_len, inputLen, &aesKey, iv, eCount, &num);
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

    AES_ctr128_encrypt(input, output, inputLen, &aesKey, iv, eCount, &num);
}
