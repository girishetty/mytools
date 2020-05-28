
#ifndef __HDCP_2X__
#define __HDCP_2X__

#include "ctypes.h"

/*
 * Generates XOR result of two data, first argument is always secured
 * 2nd argument can be secured or not based on the u2ndDataFlag
 *
 * Arguments:
 *   pbA          (IN)  - input cipher data
 *   pbB          (IN)  - input cipher/plain data
 *   cbData       (IN)  - length of pbA and pbB
 *   u2ndDataFlag (IN)  - if 0, pbB is plain, or pbB is secured
 *   pbOut        (OUT) - output cipher
 *
 * Return:
 *         STATUS_SUCCESS or one of the BCM error code
 */
STATUS HDCP2X_XOR_Cipher(UNSG8* pbA, UNSG8* pbB, UNSG32 cbData, UNSG32 u2ndDataFlag, UNSG8* pbOut);

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
STATUS HDCP2X_XOR(UINT8* pA, UINT8* pB, UINT32 cbData, UINT8* pOut);

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
STATUS HDCP2X_XOR_TwoInCipher(UINT8* pA, UINT8* pB, UINT32 cbData, UINT8* pOut);

/*
 * This is to generate Dkey based on Km, Rn, Counter and Rtx
 * Functionality of this API: calculate derived key k_{d} using below formula:
 *     dkey_{i} = AES-CTR {k_{m} XOR r_{n}, r_{tx}||ctr},
 *     r_{n}=0 when i=0,1 
 *
 * So, basically this function would do:
 * Dkey = AES_CTR_DECRYPT(DEC(pbKmEnc using ROMKEY) ^ pbRn, pbRtxCtr)
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

STATUS HDCP2X_Dkey(UNSG8* pbKmEnc, UNSG8* pbRn, UNSG8* pbRtxCtr, UNSG8* pbDkeyCipher);

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
STATUS HDCP2x_Get_Kd(UNSG8* pbKmEnc, UNSG8* pbRtx, UNSG8* pbRrx, UNSG8* pbKdCipher);

/*
 * This is to decrypt Km using RSA private key
 *
 * Arguments:
 *   pbDCipher  (IN)  - RSA decrypted private exponent
 *   pbNCipher  (IN)  - RSA decrypted public module
 *   pbKmCipher (IN)  - RSA encrypted km received from the Transmitter (during AKE_No_Stored_km)
 *   pbKmEnc    (OUT) - RSA (OAEP) decrypted pbKmCipher using pbDCipher and pbNCipher
 *                      this value is encrypted with ROMKEY before returning back to ARM
 *
 * Return:
 *         STATUS_SUCCESS or one of the BCM error code
 */

STATUS HDCP2X_RSA_ReturnCipher(UNSG8* pbDCipher, UNSG8* pbNCipher, UNSG8* pbKmCipher, UNSG8* pbKmEnc);

#endif //__HDCP_2X__
