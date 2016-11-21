#ifndef _COMMON_HEADER_H
#define _COMMON_HEADER_H

#include <stdio.h>
#include <openssl/bn.h>
#include <openssl/rsa.h>

//#define DEBUG
#ifdef DEBUG
void printbin(const char* what, const unsigned char* x, int n);
#else
#define printbin(...)
#endif

int hex2bin(unsigned char* x, const const char* s);

//#define USE_RSA_API
#define RSA_PUBLIC_ENCRYPT  0
#define RSA_PRIVATE_DECRYPT 1
#define RSA_PRIVATE_ENCRYPT 2
#define RSA_PUBLIC_DECRYPT  3

//#define IMPLEMENT_RSA_PSS

RSA* GetRsaKey(const unsigned int E, const unsigned char* pN, const unsigned char* pD);
int RsaOperation(int op, unsigned int uSize, RSA* rsa, unsigned char* pInput, unsigned char* pOutput);

#endif
