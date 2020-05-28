#ifndef _HDCP_2X_API_H
#define _HDCP_2X_API_H

typedef struct _HDCP2x_CTX {
    //plain keys from the key store
    UINT8 lc128[16];
    UINT8 dn[256];

    //secure keys from the key store
    UINT8 elc128[16];
    UINT8 edn[256];

    UINT8 rrx[8];
    UINT8 rtx[8];
    UINT8 rn[8];

    UINT8 ekpub_km[128];
    UINT8 ekh_km[16];
    UINT8 m[16];

    //Plain Keys
    UINT8 km[16];
    UINT8 kdmac[32];
    UINT8 kdlmac[32];

    //Secure Keys
    UINT8 ekm[16];
    UINT8 ekdmac[32];
    UINT8 ekdlmac[32];

    //ivector
    UINT8 dkey_ivec[16];

    //session key that we receive at SKE phase
    UINT8 ks[16];
    UINT8 eks[16];
    UINT8 riv[8];

    //HDCP Content Key
    UINT8 key[16];
    UINT8 ekey[16];
} HDCP2x_CTX;

int HDCP2_Set_Enc_Km(UINT8* ekpub_km, HDCP2x_CTX* pctx);

int HDCP2_Set_KHEnc_Km(UINT8* ekh_km, UINT8* m, HDCP2x_CTX* pctx);

int HDCP2_Get_kd_Handle(HDCP2x_CTX* pctx);

int HDCP2_Get_H_Prime(HDCP2x_CTX* pctx, UINT8* pHPrime);

int HDCP2_Get_L_Prime(HDCP2x_CTX* pctx, UINT8* pLPrime);

int HDCP2_Set_ks(UINT8* pEncKs, UINT8* pRIV, HDCP2x_CTX* pctx);

int HDCP2_Prepare_ContentKey(HDCP2x_CTX* pctx);

#endif
