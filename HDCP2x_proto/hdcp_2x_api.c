#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <openssl/aes.h>
#include "ctypes.h"
#include "hdcp_2x_api.h"
#include "hdcp_2x.h"
#include "hmac_sha.h"

////////////////////////////////////////////////////////////////////////////////
//! \brief Function: HDCP2_Set_Enc_Km
//!
//! Description: Decrypt a RSA encrypted Km, return its handle
//!
//!
//! \param
//!                pEnkm[IN]:  Pointer to the buffer, holding the RSA-encrypted Km.
//!                  Note, the caller should ensure the encrypted data is 1024 bit long
//!       PriKeyHandle[IN]: handle to the RSA private key .
//!          pKmHandle[OUT] Returned Km handle
//! \return Return:      S_OK if ok other: please see file ErrorCode.h
////////////////////////////////////////////////////////////////////////////////
int HDCP2_Set_Enc_Km(UINT8* ekpub_km, HDCP2x_CTX* pctx)
{
    int lRes = HDCP2X_RSA_ReturnCipher(pctx->edn, pctx->edn + 128, ekpub_km, pctx->ekm);

    return lRes;
}

////////////////////////////////////////////////////////////////////////////////
//! \brief Function:    HDCP2_Set_HKEnc_Km
//!
//! Description: Decrypt a k_{kh}-encrypted master key Km, return decrypted Km Handle
//! Note , E_{kh}(km)=k_{m}XOR AES(k_{h},m)=k_{m} XOR AES (Sha256(k_{kpriv_rx}),m)
//!
//! \param
//!                pEnkm[IN]:Pointer to the buffer, holding the encrypted Km. Note,
//!                 the caller should ensure the encrypted data is 128 bit long
//!              pMvalue[IN]:  m value
//!      hKhHandle[IN]: handle to the kh
//!          pKmHandle[OUT]: Returned Km handle
//! \return Return:      S_OK if ok other: please see file ErrorCode.h
////////////////////////////////////////////////////////////////////////////////
int HDCP2_Set_KHEnc_Km(UINT8* ekh_km, UINT8* m, HDCP2x_CTX* pctx)
{
    memcpy(pctx->ekm, ekh_km, 16);
}
////////////////////////////////////////////////////////////////////////////////
//! \brief Function:    HDCP2_Get_Derived_KeyHandle
//!
//! Description: calculate derived key k_{d}
//! Note, k_{d}=dkey_{0} || dkey_{1},   dkey_{i}     =AES-CTR{k_{m}XOR r_{n},r_{tx}||ctr},
//!                                                        r_{n}=0 when i=0,1
//!
//! \param
//!          hKmHandle[IN]: Handle to Km
//!                      rtx[IN]: 64 bit random number from transmitter.
//!pDerivedHandle[IN]: handle to derived key k_{d}
//! \return Return:      S_OK if ok other: please see file ErrorCode.h
////////////////////////////////////////////////////////////////////////////////

int HDCP2_Get_kd_Handle(HDCP2x_CTX* pctx)
{
    STATUS lRes = STATUS_SUCCESS;
    UINT8 ekd[32] = {0};
    //By default we dont use rrx while calculating dkey
    UINT8* pRrx = NULL;
#ifdef HDCP_2_2_ENABLED
    //If HDCP2.2 is enabled and the version is 2.2, then we need to use rrx
    if(g_RecvObj.TransVer == 0x2) {
        pRrx = pctx->rrx;
    }
#endif
    //Now, get the 32 byte key derivation
    lRes = HDCP2x_Get_Kd(pctx->ekm, pctx->rtx, pRrx, ekd);

    //Save the encrypted kd for calculating H Prime
    memcpy(pctx->ekdmac, ekd, 32);

    //Prepare the HMAC-SHA256 key for LC Prime, which is kd XOR rrx
    // kd = dkey0 || dkey1, which we have it ready already in ekd
    UINT8 tempRrx[32] = { 0x00 };
    memset(tempRrx, 0, 32);
    memcpy(tempRrx + 24, pctx->rrx, 8);
    HDCP2X_XOR_Cipher(ekd, tempRrx, 32, 0, pctx->ekdlmac);

    hex_print("ekdmac", pctx->ekdmac, 32);
    hex_print("ekdlmac", pctx->ekdlmac, 32);

    return lRes;
}

////////////////////////////////////////////////////////////////////////////////
//! \brief Function:    HDCP2_Get_H_Prime
//!
//! Description: Compute and output H'
//! Note, H' = HMAC-SHA256(r_{tx} XOR REPEATER, kd)
//!
////////////////////////////////////////////////////////////////////////////////
int HDCP2_Get_H_Prime(HDCP2x_CTX* pctx, UINT8* pHPrime)
{
    STATUS lRes = STATUS_SUCCESS;
    HDCP2_Get_kd_Handle(pctx);

#ifdef HDCP_2_2_ENABLED
    UINT8 pbHMAC[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    //pbHMAC[0] remains 0x00
    pbHMAC[1] = (UINT8)(g_RecvObj.TransCapMask & 0x00ff);
    pbHMAC[2] = (UINT8)((g_RecvObj.TransCapMask & 0xff00) >> 8);
    pbHMAC[3] = g_RecvObj.TransVer;
    pbHMAC[4] = (UINT8)(g_RecvObj.RcvCapMask & 0x00ff);
    pbHMAC[5] = (UINT8)((g_RecvObj.RcvCapMask & 0xff00) >> 8);
    pbHMAC[6] = g_RecvObj.RcvVer;
    pbHMAC[7] = g_RecvObj.REPEATER;
    XOR(pbHMAC, pctx->rtx, 8);
    lRes = HMAC_SHA(pbHMAC, 8, pctx->ekdmac, 32, pHPrime, ALG_SHA256);
#else
    lRes = HMAC_SHA(pctx->rtx, 8, pctx->ekdmac, 32, pHPrime, ALG_SHA256);
#endif
    if (STATUS_SUCCESS != lRes) {
        printf("%s: HMAC_SHA Failed!\n", __FUNCTION__);
    }

    return lRes;
}

////////////////////////////////////////////////////////////////////////////////
//! \brief Function:    HDCP2_Get_L_Prime
//!
//! Description: Compute and output L'
//! Note, L' = HMAC-SHA256(rn , kd XOR rrx})
//!
////////////////////////////////////////////////////////////////////////////////
int HDCP2_Get_L_Prime(HDCP2x_CTX* pctx, UINT8* pLPrime)
{
    STATUS lRes = STATUS_SUCCESS;

    // key (kd) for the hmac is already available at ekdlmac, whic is XORed with rrx
    lRes = HMAC_SHA(pctx->rn, 8, pctx->ekdlmac, 32, pLPrime, ALG_SHA256);
    if (STATUS_SUCCESS != lRes) {
        printf("%s: HMAC_SHA Failed!\n", __FUNCTION__);
    }

    return lRes;
}

/*
 *  ks = Edkey_ks XOR (dkey2 XOR rrx)
 */
int HDCP2_Set_ks(UINT8* pEdkey_ks, UINT8* pRIV, HDCP2x_CTX* pctx)
{
    STATUS lRes = STATUS_SUCCESS;
    UINT8 pbCtr[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02};
    UINT8 keydataout[16] = {0};
    UINT8 keydatarrx[16] = {0};
    UINT8 pbRtxCtr[16] = {0};

    memcpy(pbRtxCtr, pctx->rtx, 8);

#ifdef HDCP_2_2_ENABLED
    if(g_RecvObj.TransVer == 0x2) {
        UINT8 pbRrxCtr[8] = {0};
        memcpy(pbRrxCtr, pbCtr, 8);
        XOR(pbRrxCtr, pctx->rrx, 8);
        memcpy(pbRtxCtr + 8, pbRrxCtr, 8);
    } else {
        memcpy(pbRtxCtr + 8, pbCtr, 8);
    }
#else
    memcpy(pbRtxCtr + 8, pbCtr, 8);
#endif

    //Calculate dkey2
    lRes = HDCP2X_Dkey(pctx->ekm, pctx->rn, pbRtxCtr, keydataout);

    if(lRes == STATUS_SUCCESS) {
        memset(keydatarrx, 0, 16);
        memcpy(keydatarrx + 8, pctx->rrx, 8);

        //dkey2 XOR rrx
        lRes = HDCP2X_XOR(keydataout, keydatarrx, 16, keydataout);

        //ks = pEdkey_ks XOR (dkey2 XOR rrx)
        lRes = HDCP2X_XOR(keydataout, pEdkey_ks, 16, pctx->eks);

        memcpy(pctx->riv, pRIV, 8);
    } else {
        printf("%s: Failed to call HDCP2x_DKey\n", __FUNCTION__);
    }

    return lRes;
}

int HDCP2_Prepare_ContentKey(HDCP2x_CTX* pctx)
{
    STATUS lRes = STATUS_SUCCESS;

    //content key = ks XOR lc128
    lRes = HDCP2X_XOR_TwoInCipher(pctx->eks, pctx->elc128, 16, pctx->ekey);

    //Load the key to BCM's TSP

    return lRes;
}

