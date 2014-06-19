#include <stdlib.h>
#include <stdio.h>
#include <openssl/aes.h>
#include "common_def.h"

//#define SSL_ENCRYPTION

void aes_cbc_encrypt_(const UINT8* input, UINT32 inputLen, const UINT8* key, UINT8* iv, UINT8* output, UINT32* outLen);
void aes_cbc_decrypt_(const UINT8* input, UINT32 inputLen, const UINT8* key, UINT8* iv, UINT8* output, UINT32* outLen);

int aes_cbc(const UINT8* input, UINT32 inputLen, const UINT8* key, UINT8* iv, UINT8* output, UINT32* outLen, int mode) {
    int res = SUCCESS;
    if(mode == AES_ENCRYPT) {
        aes_cbc_encrypt_(input, inputLen, key, iv, output, outLen);
    } else if (mode == AES_DECRYPT) {
        aes_cbc_decrypt_(input, inputLen, key, iv, output, outLen);
    } else {
        res = FAILURE;
    }

    return res;
}


//This independent key is used for encrypting the IV for the first time
const UINT8 IV_KEY[] = {0x87, 0x41, 0xAC, 0x11, 0xBE, 0xFB, 0x67, 0x30, 0x49, 0xEC, 0x4C, 0x5A, 0x6D, 0xF1, 0x07, 0xf9};

const UINT8 pad[] =    {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10};


const unsigned char* XOR_strings_with_len(const unsigned char* str1, const unsigned char* str2, unsigned int len) {
    unsigned int xor_len = len;
    unsigned int index = 0;

    unsigned char* xored = (unsigned char*) malloc(xor_len);

    for(index = 0; index < xor_len; index ++) {
        xored[index] = str1[index] ^ str2[index];
    }

    return xored;
}

void aes_cbc_encrypt_(const UINT8* input, UINT32 inputLen, const UINT8* key, UINT8* iv, UINT8* output, UINT32* outLen) {

    int keylength = 128;  //16 byte key (128 bits)
    int iv_len = 16;      //16 byte iencryption IV, randomly selected
    const size_t encslength = ((inputLen + AES_BLOCK_SIZE) / AES_BLOCK_SIZE) * AES_BLOCK_SIZE;
    UINT8 plain_text[encslength];

    //Do the padding if the input length is not aligned with AES_BLOCK_SIZE
    memcpy(plain_text, input, inputLen);
    int padding = encslength - inputLen;
    int offset = 0;
    for(; offset < padding; offset++) {
        plain_text[inputLen + offset] = pad[padding];
    }

    //initialize the output with iv
    memcpy(output, iv, iv_len);

    AES_KEY encryptKey;
    AES_set_encrypt_key(key, keylength, &encryptKey);

#ifdef SSL_ENCRYPTION
    //encrypt
    AES_cbc_encrypt(plain_text, output + iv_len, encslength, &encryptKey, iv, AES_ENCRYPT);
#else
    //Encrypt the IV for the first time
    AES_KEY iv_encryptKey;
    AES_set_encrypt_key(IV_KEY, keylength, &iv_encryptKey);
    AES_encrypt(iv, iv, &iv_encryptKey);

    //Apply AES encryption for each AES BLOCK of input
    int index = 0;
    UINT8* inbuffer = NULL;
    for(; index < encslength; index += AES_BLOCK_SIZE) {

        //XOR IV and a block of plain text
        inbuffer = XOR_strings_with_len(iv, plain_text + index, AES_BLOCK_SIZE);

        //And now encrypt the input and copy to output
        AES_encrypt(inbuffer, output + iv_len + index, &encryptKey);
        free(inbuffer);
        iv = output + iv_len + index;
    }
    
#endif
    *outLen = encslength + iv_len;
}

void aes_cbc_decrypt_(const UINT8* input, UINT32 inputLen, const UINT8* key, UINT8* iv, UINT8* output, UINT32* outLen) {

    int keylength = 128;  //16 byte key (128 bits)
    int iv_len = 16;      //16 byte iencryption IV, randomly selected

    //decrypt
    AES_KEY decryptKey;
    AES_set_decrypt_key(key, keylength, &decryptKey);

#ifdef SSL_ENCRYPTION
    AES_cbc_encrypt(input, output, inputLen, &decryptKey, iv, AES_DECRYPT);
#else
    //Encrypt the IV for the first time
    AES_KEY iv_encryptKey;
    AES_set_encrypt_key(IV_KEY, keylength, &iv_encryptKey);
    AES_encrypt(iv, iv, &iv_encryptKey);

    //Apply AES decryption for each AES BLOCK of input
    int index = 0;
    UINT8* outbuffer = NULL;
    for(; index < inputLen; index += AES_BLOCK_SIZE) {
        //Decrypt the cipher block first
        AES_decrypt(input + index, output + index, &decryptKey);

        //Now XOR with previous cipher block (or the IV)
        outbuffer = XOR_strings_with_len(iv, output + index, AES_BLOCK_SIZE);
        iv = input + index;

        //Copy the XORed value back to output
        memcpy(output + index, outbuffer, AES_BLOCK_SIZE);
        free(outbuffer);
    }
#endif

    //During encryption, while padding, size of the padding was padded for every padded byte.
    //So, while unpadding, just truncate that many number of bytes to unpad
    int paddingSize = output[inputLen - 1];
    *outLen = inputLen - paddingSize;
}
