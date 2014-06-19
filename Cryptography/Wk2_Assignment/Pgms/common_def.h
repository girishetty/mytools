#ifndef COMMON_DEF_H
#define COMMON_DEF_H

#define SUCCESS                       0
#define FAILURE                       -1

typedef unsigned char UINT8;
typedef unsigned int  UINT32;

void hex_print(const char* what, const void* pv, size_t len);

int aes_cbc(const UINT8* input, UINT32 inputLen, const UINT8* key, UINT8* iv, UINT8* output, UINT32* outLen, int mode);
int aes_ctr(const UINT8* input, UINT32 inputLen, const UINT8* key, UINT8* iv, UINT8* output, int mode);

#endif //COMMON_DEF_H
