#include <string.h>
#include "common_header.h"

RSA* GetRsaKey(const unsigned int E, const unsigned char* pN, const unsigned char* pD)
{
    BIGNUM* n = NULL;
    BIGNUM* d = NULL;
    RSA*    rsa = NULL;

    rsa = RSA_new();
    n = BN_new();
    d = BN_new();
    if (rsa && n && d) {
        BN_hex2bn (&n, pN);
        BN_hex2bn (&d, pD);

        rsa->n = n;
        rsa->d = d;
        rsa->e = BN_new();
        BN_set_word(rsa->e, E);

        //printf("GetRsaKey: RSA KeySize(%d) bits\n", BN_num_bits(rsa->n));
    }

    return rsa;
}

//output = input power E mod N
int RsaOperation(int op, unsigned int uSize, RSA* rsa, unsigned char* pInput, unsigned char* pOutput)
{
    int res = 0;
    BN_CTX* ctx = NULL;
    BIGNUM* output = NULL;
    BIGNUM* input = NULL;

    output = BN_new();
    input =  BN_new();
    ctx = BN_CTX_new();

    BN_bin2bn(pInput, uSize, input);
    if (op == RSA_PRIVATE_DECRYPT || op == RSA_PRIVATE_ENCRYPT) {
        res = BN_mod_exp(output, input, rsa->d, rsa->n, ctx);
    } else {
        res = BN_mod_exp(output, input, rsa->e, rsa->n, ctx);
    }

    if (res == 1) {
        int len = BN_num_bytes(output);
        int diff = uSize - len;
        if (diff) {
            memset(pOutput, 0, uSize - len);
        }
        len = BN_bn2bin(output, pOutput + diff);
        res = uSize;
    }

    BN_free(output);
    BN_free(input);
    BN_CTX_free(ctx);
    return res;
}
