/*
 * RSA-PSS Signature Verification test code based on KMv1 test vectors
 */

#include <openssl/err.h>
#include <openssl/evp.h>
#include "common_header.h"

#ifdef IMPLEMENT_RSA_PSS
const unsigned char zeroes[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

#if defined(_MSC_VER) && defined(_ARM_)
# pragma optimize("g", off)
#endif

int RSA_verify_PKCS1_PSS(RSA *rsa, const unsigned char *mHash,
                         const EVP_MD *Hash, const unsigned char *EM,
                         int sLen)
{
    return RSA_verify_PKCS1_PSS_mgf1(rsa, mHash, Hash, NULL, EM, sLen);
}

int RSA_verify_PKCS1_PSS_mgf1(RSA *rsa, const unsigned char *mHash,
                              const EVP_MD *Hash, const EVP_MD *mgf1Hash,
                              const unsigned char *EM, int sLen)
{
    int i;
    int ret = 0;
    int hLen, maskedDBLen, MSBits, emLen;
    const unsigned char *H;
    unsigned char *DB = NULL;
    EVP_MD_CTX ctx;
    unsigned char H_[EVP_MAX_MD_SIZE];
    EVP_MD_CTX_init(&ctx);

    if (mgf1Hash == NULL)
        mgf1Hash = Hash;

    hLen = EVP_MD_size(Hash);
    if (hLen < 0)
        goto err;
    /*-
     * Negative sLen has special meanings:
     *      -1      sLen == hLen
     *      -2      salt length is autorecovered from signature
     *      -N      reserved
     */
    if (sLen == -1)
        sLen = hLen;
    else if (sLen == -2)
        sLen = -2;
    else if (sLen < -2) {
        RSAerr(RSA_F_RSA_VERIFY_PKCS1_PSS_MGF1, RSA_R_SLEN_CHECK_FAILED);
        goto err;
    }

    MSBits = (BN_num_bits(rsa->n) - 1) & 0x7;
    emLen = RSA_size(rsa);
    //printf("%s: MSBits(%d) emLen(%d)\n", __FUNCTION__, MSBits, emLen);
    if (EM[0] & (0xFF << MSBits)) {
        RSAerr(RSA_F_RSA_VERIFY_PKCS1_PSS_MGF1, RSA_R_FIRST_OCTET_INVALID);
        goto err;
    }
    if (MSBits == 0) {
        EM++;
        emLen--;
    }
    if (emLen < (hLen + sLen + 2)) { /* sLen can be small negative */
        RSAerr(RSA_F_RSA_VERIFY_PKCS1_PSS_MGF1, RSA_R_DATA_TOO_LARGE);
        goto err;
    }
    if (EM[emLen - 1] != 0xbc) {
        RSAerr(RSA_F_RSA_VERIFY_PKCS1_PSS_MGF1, RSA_R_LAST_OCTET_INVALID);
        goto err;
    }
    maskedDBLen = emLen - hLen - 1;
    H = EM + maskedDBLen;
    DB = OPENSSL_malloc(maskedDBLen);
    if (!DB) {
        RSAerr(RSA_F_RSA_VERIFY_PKCS1_PSS_MGF1, ERR_R_MALLOC_FAILURE);
        goto err;
    }
    if (PKCS1_MGF1(DB, maskedDBLen, H, hLen, mgf1Hash) < 0)
        goto err;
    for (i = 0; i < maskedDBLen; i++)
        DB[i] ^= EM[i];
    if (MSBits)
        DB[0] &= 0xFF >> (8 - MSBits);
    for (i = 0; DB[i] == 0 && i < (maskedDBLen - 1); i++) ;
    if (DB[i++] != 0x1) {
        RSAerr(RSA_F_RSA_VERIFY_PKCS1_PSS_MGF1, RSA_R_SLEN_RECOVERY_FAILED);
        goto err;
    }
    if (sLen >= 0 && (maskedDBLen - i) != sLen) {
        RSAerr(RSA_F_RSA_VERIFY_PKCS1_PSS_MGF1, RSA_R_SLEN_CHECK_FAILED);
        goto err;
    }
    if (!EVP_DigestInit_ex(&ctx, Hash, NULL)
        || !EVP_DigestUpdate(&ctx, zeroes, sizeof zeroes)
        || !EVP_DigestUpdate(&ctx, mHash, hLen))
        goto err;
    if (maskedDBLen - i) {
        if (!EVP_DigestUpdate(&ctx, DB + i, maskedDBLen - i))
            goto err;
    }
    //printf("%s: emLen(%d) sLen(%d) hLen(%d) maskLen(%d) psLen(%d) msgLen(%lu)\n", __FUNCTION__, emLen, maskedDBLen - i, hLen, maskedDBLen, (i-1), sizeof(zeroes) + hLen + maskedDBLen - i);
    if (!EVP_DigestFinal_ex(&ctx, H_, NULL))
        goto err;
    if (memcmp(H_, H, hLen)) {
        RSAerr(RSA_F_RSA_VERIFY_PKCS1_PSS_MGF1, RSA_R_BAD_SIGNATURE);
        ret = 0;
    } else
        ret = 1;

 err:
    if (DB)
        OPENSSL_free(DB);
    EVP_MD_CTX_cleanup(&ctx);

    printf("RSA_verify_PKCS1_PSS_mgf1: Done with %d\n", ret);
    return ret;
}

int RSA_padding_add_PKCS1_PSS(RSA *rsa, unsigned char *EM,
                              const unsigned char *mHash,
                              const EVP_MD *Hash, int sLen)
{
    return RSA_padding_add_PKCS1_PSS_mgf1(rsa, EM, mHash, Hash, NULL, sLen);
}

int RSA_padding_add_PKCS1_PSS_mgf1(RSA *rsa, unsigned char *EM,
                                   const unsigned char *mHash,
                                   const EVP_MD *Hash, const EVP_MD *mgf1Hash,
                                   int sLen)
{
    int i;
    int ret = 0;
    int hLen, maskedDBLen, MSBits, emLen;
    unsigned char *H, *salt = NULL, *p;
    EVP_MD_CTX ctx;

    if (mgf1Hash == NULL)
        mgf1Hash = Hash;

    hLen = EVP_MD_size(Hash);
    if (hLen < 0)
        goto err;
    /*-
     * Negative sLen has special meanings:
     *      -1      sLen == hLen
     *      -2      salt length is maximized
     *      -N      reserved
     */
    if (sLen == -1)
        sLen = hLen;
    else if (sLen == -2)
        sLen = -2;
    else if (sLen < -2) {
        RSAerr(RSA_F_RSA_PADDING_ADD_PKCS1_PSS_MGF1, RSA_R_SLEN_CHECK_FAILED);
        goto err;
    }

    MSBits = (BN_num_bits(rsa->n) - 1) & 0x7;
    emLen = RSA_size(rsa);
    //printf("%s: MSBits(%d) emLen(%d)\n", __FUNCTION__, MSBits, emLen);
    if (MSBits == 0) {
        *EM++ = 0;
        emLen--;
    }
    if (sLen == -2) {
        sLen = emLen - hLen - 2;
    } else if (emLen < (hLen + sLen + 2)) {
        RSAerr(RSA_F_RSA_PADDING_ADD_PKCS1_PSS_MGF1,
               RSA_R_DATA_TOO_LARGE_FOR_KEY_SIZE);
        goto err;
    }
    if (sLen > 0) {
        salt = OPENSSL_malloc(sLen);
        if (!salt) {
            RSAerr(RSA_F_RSA_PADDING_ADD_PKCS1_PSS_MGF1,
                   ERR_R_MALLOC_FAILURE);
            goto err;
        }
        if (RAND_bytes(salt, sLen) <= 0)
            goto err;
    }
    maskedDBLen = emLen - hLen - 1;
    H = EM + maskedDBLen;
    EVP_MD_CTX_init(&ctx);
    if (!EVP_DigestInit_ex(&ctx, Hash, NULL)
        || !EVP_DigestUpdate(&ctx, zeroes, sizeof zeroes)
        || !EVP_DigestUpdate(&ctx, mHash, hLen))
        goto err;
    if (sLen && !EVP_DigestUpdate(&ctx, salt, sLen))
        goto err;
    if (!EVP_DigestFinal_ex(&ctx, H, NULL))
        goto err;
    EVP_MD_CTX_cleanup(&ctx);

    //printf("%s: emLen(%d) sLen(%d) hLen(%d) maskLen(%d) psLen(%d) msgLen(%lu)\n", __FUNCTION__, emLen, sLen, hLen, maskedDBLen, (emLen -sLen -hLen -2), sizeof(zeroes) + hLen + sLen);

    /* Generate dbMask in place then perform XOR on it */
    if (PKCS1_MGF1(EM, maskedDBLen, H, hLen, mgf1Hash))
        goto err;

    p = EM;

    /*
     * Initial PS XORs with all zeroes which is a NOP so just update pointer.
     * Note from a test above this value is guaranteed to be non-negative.
     */
    p += emLen - sLen - hLen - 2;
    *p++ ^= 0x1;
    if (sLen > 0) {
        for (i = 0; i < sLen; i++)
            *p++ ^= salt[i];
    }
    if (MSBits) {
        //printf("%s: EM[0](%x) : ", __FUNCTION__, EM[0]);
        EM[0] &= 0xFF >> (8 - MSBits);
        //printf("EM[0](%x)\n", EM[0]);
    }

    /* H is already in place so just set final 0xbc */

    EM[emLen - 1] = 0xbc;

    ret = 1;

 err:
    if (salt)
        OPENSSL_free(salt);

    printf("%s: Done with %d\n", __FUNCTION__, ret);
    return ret;
}
#endif //IMPLEMENT_RSA_PSS
