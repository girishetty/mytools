#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/x509.h>
#include <openssl/hmac.h>

typedef unsigned char UCHAR;
typedef unsigned int  UINT32;

#define ONE_BLOCK     1024

#ifndef SHA256_DIGEST_LENGTH
#define SHA256_DIGEST_LENGTH 32
#endif // SHA256_DIGEST_LENGTH

const UCHAR* ReadFromFile(const char* fName, UINT32* pFileSize) {
    UINT32 uFileSize = 0;
    UINT32 uReadSize = 0;
    UCHAR* pFileBuf = NULL;

    FILE* fp = fopen(fName, "rb");
    if (fp) {
        //Get the File Size
        fseek(fp, 0, SEEK_END);
        uFileSize = ftell(fp);
        fseek(fp, 0, SEEK_SET);
        pFileBuf = (UCHAR*) malloc(uFileSize);
        if (pFileBuf) {
            uReadSize = fread(pFileBuf, 1, uFileSize, fp);
            *pFileSize = uReadSize;
        } else {
            printf("File Read Buffer Allocation Failed!\n");
        }
        if(uReadSize != uFileSize) {
            printf("File Read Failed!\n");
            free(pFileBuf);
            pFileBuf = NULL;
        }
        fclose(fp);
    } else {
        printf("File (%s) Open Failed!\n", fName);
    }
    return pFileBuf;
}

void GenerateHash(const char* fName, UCHAR* pHash) {
    UCHAR dataBlock[ONE_BLOCK + SHA256_DIGEST_LENGTH] = { 0 };
    UCHAR hash[SHA256_DIGEST_LENGTH] = { 0 };
    UINT32 uFileSize = 0;
    const UCHAR* pFileBuf = ReadFromFile(fName, &uFileSize);

    //Start from the end of the file, generate the hash, and work your way back to begining of the file
    UINT32 loopCount = uFileSize >> 10;
    UINT32 lastBlockSize = uFileSize & 0x3FF;
    UINT32 lastBlockOffset = uFileSize - lastBlockSize;
    UINT32 nextBlockOffset = 0;
    SHA256(pFileBuf + lastBlockOffset, lastBlockSize, hash);

    for(; loopCount != 0; loopCount --) {
        nextBlockOffset = (loopCount - 1) << 10;
        memcpy(dataBlock, pFileBuf + nextBlockOffset, ONE_BLOCK);
        memcpy(dataBlock + ONE_BLOCK, hash, SHA256_DIGEST_LENGTH);
        SHA256(dataBlock, ONE_BLOCK + SHA256_DIGEST_LENGTH, hash);
    }

    memcpy(pHash, hash, SHA256_DIGEST_LENGTH);

    free ((void*)pFileBuf);
}

void DisplayHash(const char* fName, const UCHAR* hash) {
    int idx = 0;

    printf("File Name: %s\nHash Tag: \n", fName);
    for(; idx < 32; idx++)
        printf("%02x", hash[idx]);
    printf("\n");
}

int main(int argc, char *argv[]) {
    const char* input_file = NULL;
    UCHAR hash[SHA256_DIGEST_LENGTH];

    if (argc == 2) {
        input_file = argv[1];
        printf("Input file name: %s\n", input_file);
        GenerateHash(input_file, hash);
        DisplayHash(input_file, hash);
    } else {
        printf("No Input file in the argument list!\n");
    }
    return 0;
}
