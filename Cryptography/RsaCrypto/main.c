#include <string.h>
#include "common_header.h"
#include "test_vectors.h"

int digit(char c)
{
    return c >= '0' && c <= '9' ? c - '0' : (c & ~0x20) - ('A' - 10);
}

int hex2bin(unsigned char* x, const const char* s)
{
    int i = 0;
    for (i = 0; s[0] &&s [1]; s += 2) {
        x[i++] = digit(s[0]) << 4 | digit(s[1]);
    }

    return i;
}

#ifdef DEBUG
void printbin(const char* what, const unsigned char* x, int n)
{
    printf("%s(%d): ", what, n);
    while (n--) {
        printf("%02x", *x++);
    }

    printf("\n");
}
#endif


void RsaCryptoVerificaiton(unsigned int uKeySize, const unsigned char* pMsg,
                           unsigned int E, const unsigned char* pN, const unsigned char* pD);
void RsaPssVerification(unsigned int uKeySize, const unsigned char* pMsg, const unsigned char* pSign,
                       unsigned int E, const unsigned char* pN, const unsigned char* pD);

int main ()
{
    RsaPssVerification(256, long_kat_hash, long_kat_sign, E, kat_rsa_n, kat_rsa_d);
    RsaPssVerification(256, smal_kat_hash, smal_kat_sign, E, kat_rsa_n, kat_rsa_d);
    RsaPssVerification(64, HighestPriorityProvider_hash, HighestPriorityProvider_sign, E, HighestPriorityProvider_rsa_n, HighestPriorityProvider_rsa_d);
    RsaCryptoVerificaiton(256, M, E, N, D);

    return 0;
}
