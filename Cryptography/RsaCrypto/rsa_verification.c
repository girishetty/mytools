#include <string.h>
#include "common_header.h"

void RsaCryptoVerificaiton(unsigned int uKeySize, const unsigned char* pMsg,
                           unsigned int E, const unsigned char* pN, const unsigned char* pD)
{
    unsigned char xx[512];
    unsigned char yy[512];
    unsigned char zz[512];
    int t = 0;
    RSA* rsa = NULL;

    rsa = GetRsaKey(E, pN, pD);
    if (rsa) {
        t = hex2bin (xx, pMsg);
        printbin ("Msg", xx, t);

#ifdef USE_RSA_API
        t = RSA_private_encrypt (t, xx, yy, rsa, RSA_NO_PADDING);
#else
        RsaOperation(RSA_PRIVATE_ENCRYPT, t, rsa, xx, yy);
#endif
        printbin ("Cipher", yy, t);

#ifdef USE_RSA_API
        t = RSA_public_decrypt (t, yy, zz, rsa, RSA_NO_PADDING);
#else
        RsaOperation(RSA_PUBLIC_DECRYPT, t, rsa, yy, zz);
#endif
        printbin ("Clear", zz, t);
        if (memcmp(xx, zz, t) != 0) {
            printf("Sign/Verify FAILED!!\n");
        } else {
            printf("Sign/Verify SUCCESS\n");
        }

#ifdef USE_RSA_API
        t = RSA_public_encrypt (t, xx, yy, rsa, RSA_NO_PADDING);
#else
        RsaOperation(RSA_PUBLIC_ENCRYPT, t, rsa, xx, yy);
#endif
        printbin ("Cipher", yy, t);

#ifdef USE_RSA_API
        t = RSA_private_decrypt (t, yy, zz, rsa, RSA_NO_PADDING);
#else
        RsaOperation(RSA_PRIVATE_DECRYPT, t, rsa, yy, zz);
#endif
        printbin ("Clear", zz, t);
        if (memcmp(xx, zz, t) != 0) {
            printf("Encrypt/Decrypt FAILED!!\n");
        } else {
            printf("Encrypt/Decrypt SUCCESS\n");
        }
        RSA_free(rsa);
    }
}
