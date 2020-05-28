#ifndef _HDCP_RECEIVER_H
#define _HDCP_RECEIVER_H

#include "ctypes.h"

int setup_rsa_key();

void clean_rsa_key();

void ake_init_messageHandler(HDCP2x_CTX* pctx, UINT8* r_tx);

void ake_no_stored_km_messageHandler(HDCP2x_CTX* pctx, UINT8* ekpub_km);

void ake_stored_km_messageHandler(HDCP2x_CTX* pctx, UINT8* ekh_km, UINT8* m);

int ake_send_h_prime_init(HDCP2x_CTX* pctx, UINT8* pHPrime, BOOL isStoredKm);

int lc_send_l_prime_init(HDCP2x_CTX* pctx, UINT8* pLPrime);

void ske_send_eks_messageHandler(UINT8* pEdkey_ks, UINT8* pRIV, HDCP2x_CTX* pctx);

void set_hdcp_content_key(HDCP2x_CTX* pctx);

#endif

