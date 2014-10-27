#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <openssl/aes.h>
#include <openssl/hmac.h>
#include <openssl/bn.h>
#include <openssl/rsa.h>
#include "hdcp_2x.h"
#include "aes_algo.h"

//#define LOG

#define IN_BUF_SIZE  1024
#define OUT_BUF_SIZE 1024

#define PLAIN_DATA     0x00
#define SECURE_DATA    0x01

UNSG8 in_buf[IN_BUF_SIZE];
UNSG8 out_buf[OUT_BUF_SIZE];
UNSG8 iv_buf[16] = { 0x00 };
#if 0
const UNSG8 ROM_KEY[16] = { 0x5c, 0x3e, 0x8a, 0xd0, 0x0d, 0x4f, 0x2b, 0x4c,
                            0xb3, 0xc8, 0xfe, 0x22, 0x9a, 0x84, 0x63, 0xbd };
#else
const UNSG8 ROM_KEY[16] = { 0x7a, 0xcd, 0x2c, 0x9c, 0xb8, 0x32, 0x37, 0x1c,
                            0xba, 0x4f, 0x63, 0x20, 0x73, 0xb6, 0xa9, 0xc7 };
#endif

#define HDCP_DN_KEY   0x01
#define HDCP_LC128    0x02
#define HDCP_PRIV_KEY 0x03

#define HDCP_DN_SECURE_KEY_SIZE    384
#define HDCP_LC128_SECURE_KEY_SIZE 144
#define HDCP_PRIV_SECURE_KEY_SIZE  480

#define HDCP_DN_KEY_SIZE    256
#define HDCP_LC128_KEY_SIZE 16
#define HDCP_PRIV_KEY_SIZE  340

void XOR_Size(UNSG8* a, UNSG8* b, UNSG8* out, UNSG32 uLength)
{
    register SIGN32 i = 0;
    register UNSG32* pa = (UNSG32*)a;
    register UNSG32* pb = (UNSG32*)b;
    register UNSG32* pout = (UNSG32*)out;
    register UNSG32 uCount = uLength/4;

#ifdef LOG
    hex_print("A", a, uLength);
    hex_print("B", b, uLength);
#endif
    //XOR 4 bytes at a time (size of UNSG32)
    for (i=0; i < uCount; i++) {
        pout[i] = pa[i] ^ pb[i];
    }

    //If data is not 4 bytes aligned, then XOR remaining data
    i = i * 4;
    for (; i < uLength; i++) {
        out[i] = a[i] ^ b[i];
    }
#ifdef LOG
    hex_print("A XOR B", out, uLength);
#endif
}

/*
 * Generates XOR result of two data, first argument is always secured
 * 2nd argument can be secured or not based on the u2ndDataFlag
 *
 * Arguments:
 *   pbA          (IN)  - input cipher data
 *   pbB          (IN)  - input plain data
 *   cbData       (IN)  - length of pbA and pbB
 *   u2ndDataFlag (IN)  - if 0, pbB is plain, or pbB is secured
 *   pbOut        (OUT) - output cipher
 *
 * Return:
 *         STATUS_SUCCESS or one of the BCM error code
 */
STATUS HDCP2X_XOR_Cipher(UNSG8* pbA, UNSG8* pbB, UNSG32 cbData, UNSG32 u2ndDataFlag, UNSG8* pbOut)
{
    STATUS status = STATUS_SUCCESS;
    //By default we are using ROM_KEY
    UNSG8* key = (UNSG8*)ROM_KEY;
    UNSG8 inputA[IN_BUF_SIZE];
    UNSG8 inputB[IN_BUF_SIZE];
    UNSG8* pSecond = pbB;

    //First input data is protected, decrypt it before using
    status = aes_process(pbA, inputA, NULL, key, 128, cbData, AES_DECRYPT, AES_CBC);

    if (status == STATUS_SUCCESS) {
        //Copy the 2nd input data from SoC to BCM
        if (u2ndDataFlag) {
            pSecond = inputB;
        }
        if (u2ndDataFlag == SECURE_DATA) {
            //Second input data is protected, decrypt it before using
            status = aes_process(pbB, inputB, NULL, key, 128, cbData, AES_DECRYPT, AES_CBC);
        }

        //Perform XOR of A and B now
        XOR_Size(inputA, pSecond, in_buf, cbData);
        //Encrypt the XORed data, before sending it to ARM side
        status = aes_process(in_buf, pbOut, NULL, key, 128, cbData, AES_ENCRYPT, AES_CBC);

        if (status != STATUS_SUCCESS) {
            printf("%s: Output XOR Failed!\n", __FUNCTION__);
        }
    }
    return status;
}

/*
 * Function: HDCP2X_XOR
 *
 * Generates XOR result of two data, first argument is secured
 * 2nd argument is plain
 *
 * Arguments:
 *   pA           (IN)  - input cipher data
 *   pB           (IN)  - input plain data
 *   cbData       (IN)  - length of A and B
 *   pOut         (OUT) - output cipher
 *
 * Return:
 *   SUCCESS or one of the error code
 */
STATUS HDCP2X_XOR(UINT8* pA, UINT8* pB, UINT32 cbData, UINT8* pOut)
{
    STATUS lRes = STATUS_SUCCESS;

    lRes = HDCP2X_XOR_Cipher(pA, pB, cbData, PLAIN_DATA, pOut);

    return lRes;
}

/*
 * Function: HDCP2X_XOR_TwoInCipher
 *
 * Generates XOR result of two data, both are secured
 *
 * Arguments:
 *   pA           (IN)  - input cipher data
 *   pB           (IN)  - input cipher data
 *   cbData       (IN)  - length of pA and pB
 *   pOut         (OUT) - output cipher
 *
 * Return:
 *   SUCCESS or one of the error code
 */
STATUS HDCP2X_XOR_TwoInCipher(UINT8* pA, UINT8* pB, UINT32 cbData, UINT8* pOut)
{
    STATUS lRes = STATUS_SUCCESS;

    lRes = HDCP2X_XOR_Cipher(pA, pB, cbData, SECURE_DATA, pOut);

    return lRes;
}
/*
 * This is to generate Dkey based on Km, Rn, Counter and Rtx
 * Functionality of this API: calculate derived key k_{d} using below formula:
 *     dkey_{i} = AES-CTR {k_{m} XOR r_{n}, r_{tx}||ctr},
 *
 * Arguments:
 *   pKm      (IN)  - 128 bit km
 *   pRn      (IN)  - 64 bit pseudo random number Rn
 *   pRtxCtr  (IN)  - 128 bit rtx concatenated with Counter ctr
 *   pDkey    (OUT) - derived key dkey
 *
 * Return:
 *         STATUS_SUCCESS or one of the BCM error code
 */
 
STATUS Calculate_DKey(UNSG8* pKm, UNSG8* pRn, UNSG8* pRtxCtr, UNSG8* pDkey)
{
    STATUS status = STATUS_SUCCESS;
    UNSG8 key[16];
    UNSG8 zeros[16] = { 0x00 };
    int i = 0;

#ifdef LOG
    printf("=======Calculate_DKey=======\n");
    hex_print("km", pKm, 16);
    hex_print("rn", pRn, 8);
    hex_print("rtx||ctr", pRtxCtr, 16);
#endif
    memset(zeros, 0x00, 16);

    //key = XOR (Km, Rn)
    memcpy(key, pKm, 16);
    for (; i < 8; i++) {
        key[8+i] = key[8+i] ^ pRn[i];
    }

    status = aes_process(zeros, pDkey, pRtxCtr, key, 128, 16, AES_ENCRYPT, AES_CTR);
#ifdef LOG
    hex_print("Dkey", pDkey, 16);
#endif

    return status;
}

/*
 * This is to generate Dkey based on Km, Rn, Counter and Rtx
 * Functionality of this API: calculate derived key k_{d} using below formula:
 *     dkey_{i} = AES-CTR {k_{m} XOR r_{n}, r_{tx}||ctr},
 *     r_{n}=0 when i=0,1 
 *
 * So, basically this function would do:
 * Dkey = AES_CTR_DECRYPT(DEC(pbKmEnc using ROMKEY), pbRtxCtr)
 * pbDkeyCipher = ENC(Dkey using ROMKEY)
 * 
 * Arguments:
 *   pbKmEnc      (IN)  - 16 bytes km cipher enced with romkey or rkek
 *   pbRn         (IN)  - 8 bytes rn
 *   pbRtxCtr     (IN)  - 16 bytes rtx||ctr 
 *   pbDkeyCipher (OUT) - 16 bytes derived key D Key, encrypted with ROMKEY
 *
 * Return:
 *         STATUS_SUCCESS or one of the BCM error code
 */
 
STATUS HDCP2X_Dkey(UNSG8* pbKmEnc, UNSG8* pbRn, UNSG8* pbRtxCtr, UNSG8* pbDkeyCipher)
{
    STATUS status = STATUS_SUCCESS;
    UNSG8* key = (UNSG8*)ROM_KEY;
    UNSG8 km[16];
    UNSG8 dkey[16];

    //Copy the ROM KEY encrypted Km from SoC to BCM
    status = aes_process(pbKmEnc, km, NULL, key, 128, 16, AES_DECRYPT, AES_CBC);

    if (status == STATUS_SUCCESS) {
        //Calculate dkey
        status = Calculate_DKey(km, pbRn, pbRtxCtr, dkey);
        //Encrypt the D Key, before sending it to ARM side
        status = aes_process(dkey, pbDkeyCipher, NULL, key, 128, 16, AES_ENCRYPT, AES_CBC);

        if (status != STATUS_SUCCESS) {
            printf("%s: D key calculation Failed!\n", __FUNCTION__);
        }
    }
    return status;
}

/*  
 * This is to generate key derivation kd using dkey0 and dkey1 and based on Km, Rn, Counter and Rtx
 * Functionality of this API: calculate derived key kd using below formula:
 *     kd = dkey0 || dkey1
 *     dkey_{i} = AES-CTR {k_{m} XOR r_{n}, r_{tx}||ctr},
 *     r_{n}=0 when i=0,1 
 *  
 * Arguments:
 *   pbKmEnc      (IN)  - 16 bytes km cipher enced with romkey or rkek
 *   pbRtx        (IN)  - 8 bytes rtx 
 *   pbRrx        (IN)  - 8 bytes rrx 
 *   pbKdCipher   (OUT) - 32 bytes kd, encrypted with ROMKEY
 *  
 * Return:
 *         STATUS_SUCCESS or one of the BCM error code
 */
STATUS HDCP2x_Get_Kd(UNSG8* pbKmEnc, UNSG8* pbRtx, UNSG8* pbRrx, UNSG8* pbKdCipher)
{
    STATUS status = STATUS_SUCCESS;
    UNSG8* key = (UNSG8*)ROM_KEY;
    UNSG8 km[16];
    UNSG8 rn[8] = { 0 };
    UNSG8 rtx[8];
    UNSG8 rrx[8] = { 0 };
    UNSG8 ctr[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    UNSG8 ivector[16] = { 0 };
    UNSG8 kd[32] = { 0 };

    memset(rn, 0, 8);
    //Copy the ROM KEY encrypted Km from SoC to BCM
    status = aes_process(pbKmEnc, km, NULL, key, 128, 16, AES_DECRYPT, AES_CBC);

    if (status == STATUS_SUCCESS) {
        //First half (8 bytes) of ivector is rtx
        memcpy(ivector, pbRtx, 8);
        if (pbRrx) {
            //Its HDCP2.2, so we need to use the rrx
            memcpy(rrx, pbRrx, 8);
        } else {
            memset(rrx, 0, 8);
        }
        // XOR rrx and counter to get 2nd half of ivector
        XOR_Size(rrx, ctr, ivector + 8, 8);
        //Calculate dkey0
        status = Calculate_DKey(km, rn, ivector, kd);

        if (status == STATUS_SUCCESS) {
            //Now calculate dkey1
            ctr[7] = 0x01;
            // XOR rrx and counter to get 2nd half of ivector
            XOR_Size(rrx, ctr, ivector + 8, 8);
            status = Calculate_DKey(km, rn, ivector, kd + 16);

            if (status == STATUS_SUCCESS) {
                //Encrypt the key derivation kd
                status = aes_process(kd, pbKdCipher, NULL, key, 128, 32, AES_ENCRYPT, AES_CBC);

                if (status != STATUS_SUCCESS) {
                    printf("%s: key derivation Failed!\n", __FUNCTION__);
                }
            }
        }
    }
    return status;
}

/*
 * This is to decrypt Km using RSA private key
 *
 * Arguments:
 *   pbDCipher  (IN)  - RSA decrypted private exponent (128 bytes)
 *   pbNCipher  (IN)  - RSA decrypted public module (128 bytes)
 *   pbKmCipher (IN)  - 128 bytes of Km (RSA encrypted using Kpub) received from the Transmitter (during AKE_No_Stored_km)
 *   pbKmEnc    (OUT) - 16 bytes of ROMKEY encrypted km ==> RSA (OAEP) decrypted pbKmCipher using pbDCipher and pbNCipher
 *
 * Return:
 *         STATUS_SUCCESS or one of the BCM error code
 */
 
STATUS HDCP2X_RSA_ReturnCipher(UNSG8* pbDCipher, UNSG8* pbNCipher, UNSG8* ekpubKm, UNSG8* pbKmEnc)
{
    STATUS status = STATUS_SUCCESS;
    UNSG8 km[128];
    UINT8 rsaPrivateExpo[128];
    UINT8 rsaPublicMod[128];
    UINT32 inLen = 128;
    UINT32 outLen = 16;
    UINT32 keyLen = 128; //16 bytes
    UNSG8* key = (UNSG8*)ROM_KEY;
    RSA*         rsaKey = NULL;

    //Copy the ROM KEY encrypted RSA Private Exponent (D) from SoC to BCM and decrypt it
    status = aes_process(pbDCipher, rsaPrivateExpo, NULL, key, keyLen, inLen, AES_DECRYPT, AES_CBC);

    if (status == STATUS_SUCCESS) {
        //Copy the ROM KEY encrypted RSA Public Modulus (N) from SoC to BCM and decrypt it
        status = aes_process(pbNCipher, rsaPublicMod, NULL, key, keyLen, inLen, AES_DECRYPT, AES_CBC);

        if (status == STATUS_SUCCESS) {

            rsaKey = RSA_new();
            if(rsaKey) {

                status = key_init(rsaKey);
                if (status == STATUS_SUCCESS) {
                    status = RSA_private_decrypt_OAES_SHA256(ekpubKm, km, rsaKey);

                    if (status == STATUS_SUCCESS) {
                        //Copy the 16 bytes of Km back to SoC memory, after securing it with ROM_KEY
                        status = aes_process(km, pbKmEnc, NULL, key, keyLen, outLen, AES_ENCRYPT, AES_CBC);
                    } else {
                       printf("%s: RSA_private_decrypt_OAES_SHA256 FAILEDn", __FUNCTION__);
                    }
                }
            }
        }
    }
    return status;
}

