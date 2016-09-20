#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <openssl/hmac.h>
#include <openssl/sha.h>
#include <openssl/aes.h>
#include <openssl/bn.h>
#include <openssl/rsa.h>

#include "ctypes.h"
#include "hdcp_2x_api.h"
#include "hdcp_receiver.h"

//#define LOG

#define KEY_SIZE 862
const char* const ProdKeyFile = "hdcp_keyunit.bin";
UINT8 keyFile[KEY_SIZE] = {0x00};
RSA* rsa = NULL;

int ReadKey() {
    STATUS lRes = STATUS_SUCCESS;
    //See if there is a production key file
    FILE* key_fp = fopen(ProdKeyFile, "rb");
    if(key_fp) {
        //We have HDCP Production Key, So read it from the file now
        printf("[HDCP] ReadKey: PRODUCTION_KEY\n");
        //Now read the production key contents
        int readCount = fread(keyFile, 1, KEY_SIZE, key_fp);
        if(readCount != KEY_SIZE) {
            printf("[HDCP] ReadKey: reading from production key failed with %d\n", readCount);
            lRes = STATUS_FAILURE;
        }

        fclose(key_fp);
    }
}

int key_init(RSA* rsa)
{
    STATUS lRes = ReadKey();
    if (STATUS_SUCCESS == lRes) {

        //Copy Device Public Modules N, which is at the offset 5 (or 45) and 128 bytes
        UINT32 device_pub_module_index = 5;
        rsa->n = BN_new();
        BN_bin2bn(keyFile + device_pub_module_index, 128, rsa->n);

        //Copy Device Public Exponent E, which is at the offset 133 (or 173) and 3 bytes
        UINT32 device_pub_exp_index = 133;
        rsa->e = BN_new();
        BN_bin2bn(keyFile + device_pub_exp_index, 3, rsa->e);

        //Copy Device Private CRT P, which is at the offset 522 (or 562) and 64 bytes
        UINT32 device_pri_crt_p_index = 522;
        rsa->p = BN_new();
        BN_bin2bn(keyFile + device_pri_crt_p_index, 64, rsa->p);

        //Copy Device Private CRT Q, which is at the offset 586 (or 626) and 64 bytes
        UINT32 device_pri_crt_q_index = 586;
        rsa->q = BN_new();
        BN_bin2bn(keyFile + device_pri_crt_q_index, 64, rsa->q);

        //Copy Device Private CRT D mod (P-1), which is at the offset 650 (or 690) and 64 bytes
        UINT32 device_pri_crt_dmp1_index = 650;
        rsa->dmp1 = BN_new();
        BN_bin2bn(keyFile + device_pri_crt_dmp1_index, 64, rsa->dmp1);

        //Copy Device Private CRT D mod (Q-1), which is at the offset 714 (or 754) and 64 bytes
        UINT32 device_pri_crt_dmq1_index = 714;
        rsa->dmq1 = BN_new();
        BN_bin2bn(keyFile + device_pri_crt_dmq1_index, 64, rsa->dmq1);

        //Copy Device Private CRT q^-1 mod p, which is at the offset 778 (or 818) and 64 bytes
        UINT32 device_pri_crt_iqmp_index = 778;
        rsa->iqmp = BN_new();
        BN_bin2bn(keyFile + device_pri_crt_iqmp_index, 64, rsa->iqmp);
    }
    return lRes;
}

int setup_rsa_key() {
    STATUS lRes = STATUS_FAILURE;

    rsa = RSA_new();
    if(rsa) {
        lRes = key_init(rsa);
    }
    return lRes;
}

void clean_rsa_key() {
    if (rsa) {
        RSA_free(rsa);
        rsa = NULL;
    }
}

void init_ivec(const UINT8* rtx, UINT8* ivec) {
    memset(ivec, 0, 16);
    memcpy(ivec, rtx, 8);
}

void ake_init_messageHandler(HDCP2x_CTX* pctx, UINT8* r_tx)
{
    memcpy(pctx->rtx, r_tx, 8);
}

void ake_no_stored_km_messageHandler(HDCP2x_CTX* pctx, UINT8* ekpub_km)
{
    memcpy(pctx->ekpub_km, ekpub_km, 128);
}

void ake_stored_km_messageHandler(HDCP2x_CTX* pctx, UINT8* ekh_km, UINT8* m)
{
    memcpy(pctx->ekh_km, ekh_km, 16);
    memcpy(pctx->m, m, 16);
}

void calculate_dkey(UINT8* km, UINT8* rn, UINT8* dkey_ivec, UINT8* dkey) {
    UINT8 zeros[16] = { 0x00 };
    UINT8 key_buff[16];    // AES key in Big Endian format
    UINT8 ctr[16];
    AES_KEY key;
    UINT32 num = 0;
    int i = 0;

#ifdef LOG
    printf("=======calculate_dkey=======\n");
    hex_print("km", km, 16);
    hex_print("rn", rn, 8);
    hex_print("rtx||ctr", dkey_ivec, 16);
#endif
    memset(zeros, 0, 16);

    // Set AES key
    memcpy(key_buff, km, 16);
    XOR_Size(key_buff + 8, rn, key_buff + 8, 8);
    AES_set_encrypt_key(key_buff, 128, &key);

    AES_ctr128_encrypt(zeros, dkey, 16, &key, dkey_ivec, ctr, &num);

#ifdef LOG
    hex_print("dkey", dkey, 16);
#endif
}

int ake_send_h_prime_init(HDCP2x_CTX* pctx, UINT8* pHPrime, BOOL isStoredKm)
{
    STATUS lRes = STATUS_SUCCESS;
    UINT8* hmac_result = NULL;
    UINT8 hmac_data[8];
    int i = 0;

#if 1
    if(isStoredKm) {
        //UINT32 num = 0;
        //UINT8 ctr[16];
        //AES_ctr128_encrypt(pctx->ekh_km, pctx->km, 16, &decrKey, m, ctr, &num);
    } else{
        if (RSA_private_decrypt_OAES_SHA256(pctx->ekpub_km, pctx->km, rsa) < 0) {
            printf("%s: AKE_No_Stored_km: RSA private encrypt problem!", __FUNCTION__);
        }
    }
#endif

    memset(pctx->rn, 0, 8);
    init_ivec(pctx->rtx, pctx->dkey_ivec);

    //Save the encrypted kd for calculating H Prime
    calculate_dkey(pctx->km, pctx->rn, pctx->dkey_ivec, pctx->kdmac);
    calculate_dkey(pctx->km, pctx->rn, pctx->dkey_ivec, pctx->kdmac + 16);
    hex_print("Kd", pctx->kdmac, 32);

    //Prepare the HMAC-SHA256 key for LC Prime, which is kd XOR rrx
    //kd = dkey0 || dkey1, which we have it ready already in ekd
    memcpy(pctx->kdlmac, pctx->kdmac, 32);
    XOR_Size(pctx->kdlmac + 24, pctx->rrx, pctx->kdlmac + 24, 8);
    hex_print("Kdl", pctx->kdlmac, 32);

    //Calculate H Prime
    memcpy(hmac_data, pctx->rtx, 8);
    hmac_result = HMAC(EVP_sha256(), pctx->kdmac, 32, hmac_data, 8, NULL, NULL);
    if (hmac_result == NULL) {
        printf("%s: HMAC function problem!", __FUNCTION__);
        lRes = STATUS_FAILURE;
    } else {
        memcpy(pHPrime, hmac_result, 32);
    }

    return lRes;
}

int RSA_private_decrypt_OAES_SHA256(UINT8* ekpub_km, UINT8* km, RSA* rsa)
{
    UINT8 encrypt_padded_km[128];
    UINT8 seed[32];
    UINT8 db[95];
    int i = 0;

    int ret = RSA_private_decrypt(128, ekpub_km, encrypt_padded_km, rsa, RSA_NO_PADDING);

    memcpy(seed, encrypt_padded_km + 1, 32);
    memcpy(db, encrypt_padded_km + 33, 95);

    PKCS1_MGF1(seed, 32, db, 95, EVP_sha256());

    for(i = 0; i < 32; i++)
        seed[i] ^= encrypt_padded_km[i + 1];

    PKCS1_MGF1(db, 95, seed, 32, EVP_sha256());
    for(i = 0; i < 95; i++)
        db[i] ^= encrypt_padded_km[i + 33];

    memcpy(km, db + 95 - 16, 16);
    return STATUS_SUCCESS;
}

int lc_send_l_prime_init(HDCP2x_CTX* pctx, UINT8* pLPrime)
{
    STATUS lRes = STATUS_SUCCESS;
    UINT8* lPrime = NULL;

    // key (kd) for the hmac is already available at kdlmac, whic is XORed with rrx
    lPrime =  HMAC(EVP_sha256(), pctx->kdlmac, 32, pctx->rn, 8, NULL, NULL);

    if (lPrime == NULL) {
        printf("%s: L HMAC problem!!!", __FUNCTION__);
        lRes = STATUS_FAILURE;
    } else {
        memcpy(pLPrime, lPrime, 32);
    }
    return lRes;
}

void ske_send_eks_messageHandler(UINT8* pEdkey_ks, UINT8* pRIV, HDCP2x_CTX* pctx) {
    UINT8 dkey2[16];
    int i = 0;

    calculate_dkey(pctx->km, pctx->rn, pctx->dkey_ivec, dkey2);

    XOR_Size(dkey2 + 8, pctx->rrx, dkey2 + 8, 8);
    XOR_Size(pEdkey_ks, dkey2, pctx->ks, 16);

    memcpy(pctx->riv, pRIV, 8);
}

void set_hdcp_content_key(HDCP2x_CTX* pctx)
{
    XOR_Size(pctx->ks, pctx->lc128, pctx->key, 16);
    //AES_set_encrypt_key(buffHdcpKey, 128, &hdcpKey);
}

