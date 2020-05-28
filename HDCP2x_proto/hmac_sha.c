#include <openssl/x509.h>
#include <openssl/hmac.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hmac_sha.h"
#include "aes_algo.h"

#define MAX_HASH_TEXT_LENGTH  0x4000

extern const UNSG8 ROM_KEY[16];

/*
 *  text:      pointer to data stream
 *  text_len:  length of data stream
 *  key:       pointer to authentication key
 *  key_len:   length of authentication key
 *  digest:    caller digest to be filled in
 */
void hmac_sha256(const UINT8* text, INT32 text_len, const UINT8* key, INT32 key_len, void* digest)
{
    unsigned char k_ipad[65];   /* inner padding -
                                 * key XORd with ipad
                                 */
    unsigned char k_opad[65];   /* outer padding -
                                 * key XORd with opad
                                 */
    unsigned char tk[SHA256_DIGEST_LENGTH];
    unsigned char tk2[SHA256_DIGEST_LENGTH];
    unsigned char bufferIn[MAX_HASH_TEXT_LENGTH];
    unsigned char bufferOut[MAX_HASH_TEXT_LENGTH];
    int           i;
 
    /* if key is longer than 64 bytes reset it to key=sha256(key) */
    if ( key_len > 64 ) {
        SHA256( key, key_len, tk );
        key     = tk;
        key_len = SHA256_DIGEST_LENGTH;
    }
 
    /*
     * the HMAC_SHA256 transform looks like:
     *
     * SHA256(K XOR opad, SHA256(K XOR ipad, text))
     *
     * where K is an n byte key
     * ipad is the byte 0x36 repeated 64 times
     * opad is the byte 0x5c repeated 64 times
     * and text is the data being protected
     */
 
    /* start out by storing key in pads */
    memset( k_ipad, 0, sizeof k_ipad );
    memset( k_opad, 0, sizeof k_opad );
    memcpy( k_ipad, key, key_len );
    memcpy( k_opad, key, key_len );
 
    /* XOR key with ipad and opad values */
    for ( i = 0; i < 64; i++ ) {
        k_ipad[i] ^= 0x36;
        k_opad[i] ^= 0x5c;
    }
 
    /*
     * perform inner SHA256
     */
    memset( bufferIn, 0x00, 1024 );
    memcpy( bufferIn, k_ipad, 64 );
    memcpy( bufferIn + 64, text, text_len );

    SHA256( bufferIn, 64 + text_len, tk2 );
    /*
     * perform outer SHA256
     */
    memset( bufferOut, 0x00, 1024 );
    memcpy( bufferOut, k_opad, 64 );
    memcpy( bufferOut + 64, tk2, SHA256_DIGEST_LENGTH );

    SHA256( bufferOut, 64 + SHA256_DIGEST_LENGTH, digest );
}


STATUS HMAC_SHA(const UINT8* text, int text_len, const UINT8* key, int key_len, UINT8* digest, INT32 mode)
{
    STATUS status = STATUS_SUCCESS;
    UINT8* secureKey = (UINT8*) malloc (key_len * sizeof(UINT8));

    if (secureKey) {
        //Key is secured by ROM KEY. So, decrypt it before using
        status = aes_process((UINT8*)key, secureKey, NULL, (UINT8*)ROM_KEY, 128, key_len, AES_DECRYPT, AES_CBC);

        if (status == STATUS_SUCCESS) {
            if (mode == ALG_SHA256) {
                hmac_sha256(text, text_len, secureKey, key_len, digest);
            } else {
                status = STATUS_FAILURE;
            }
        }
    } else {
        status = STATUS_FAILURE;
    }

    return status;
}

