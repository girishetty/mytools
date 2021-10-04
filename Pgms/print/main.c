#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef unsigned int  UINT32;
typedef unsigned char UINT8;

typedef enum _AES_OP
   {
   AES_ENCRYPT,
   AES_DECRYPT,
   AES_OP_LAST_ONE
   } AES_OP;

#define DEBUG_PRINTF(...) {                            \
                                 char buffer[250];        \
                                 sprintf(buffer, __VA_ARGS__); \
                                 debug_printf(buffer);    \
                             }

void debug_printf(const char * printbuffer) {
    printf("%s", printbuffer);
}

const unsigned char sam [] = { 0x10, 0x45, 0xde, 0x56, 0xa2, 0xff, 0x18, 0xce, 0xda, 0x98 };
//const unsigned char sam [] = "Girish Shetty";

void divisibleBy4(int n) {
    printf("last 2 bits of %x is : %x\n", n, n & 0x03);

    if( (n & 0x03)){
        printf("%d is not multuple of 4\n", n);
    } else {
        printf("%d is multuple of 4\n", n);
    }
}

void pointerTest() {
#if 0
    const char* ptr1 = NULL;
    const short* ptr2 = NULL;
    const int* ptr3 = NULL;
    const float* ptr4 = NULL;
    const long long* ptr5 = NULL;

    const unsigned int size = 4;

    printf("char ptr [%x] +size [%x]\n", ptr1, ptr1 + size);
    printf("short ptr [%x] +size [%x]\n", ptr2, ptr2 + size);
    printf("int ptr [%x] +size [%x]\n", ptr3, ptr3 + size);
    printf("float ptr [%x] +size [%x]\n", ptr4, ptr4 + size);
    printf("long long ptr [%x] +size [%x]\n", ptr5, ptr5 + size);
    printf("======================================================\n");
    printf("char ptr [%x] +size [%x]\n", ptr1, ptr1 + size);
    printf("short ptr [%x] +size [%x]\n", ptr2, (const char*) ptr2 + size);
    printf("int ptr [%x] +size [%x]\n", ptr3, (const char*) ptr3 + size);
    printf("float ptr [%x] +size [%x]\n", ptr4, (const char*) ptr4 + size);
    printf("long long ptr [%x] +size [%x]\n", ptr5, (const char*) ptr5 + size);
#endif
}

void print_hex_msg(const unsigned char* str, const unsigned int len) {
    unsigned int index = 0;

    printf("HEX:");
    for(index = 0; index < len ; index ++) {
        printf("%.2x", str[index]);
        if ((index & 0xF) == 0xF) {
            //printf("\n");
        }
    }

    printf("\n");
}

void readHash() {
    FILE* fp = fopen("hash.txt", "rb");
    UINT8 hashEntry[100];
    UINT8 hash[32] = "";

    typedef union {
        UINT8  charEntry[5];
        UINT32 intEntry;
    } WORD;
    WORD word;

    UINT32 readCount = 0;
    UINT32 i = 0;
    UINT32 index = 0;
    UINT8* p = hashEntry;

    if (fp) {
        readCount = fscanf(fp, "%s", hashEntry);
        if (readCount != -1 && strlen(hashEntry) == 64) {
            printf("Hash is [%s]\n", hashEntry);

            for (i = 0; i < 16 ; i++) {
                memcpy(word.charEntry, p + (i * 4), 4);
                word.charEntry[4] = '\0';
                printf("[%x %s]: ", word.intEntry, word.charEntry);
                word.intEntry = strtol(word.charEntry, NULL, 16);
                printf("[%.4x %.2x%.2x]\n", word.intEntry, word.charEntry[1], word.charEntry[0]);
                hash[index++] = word.charEntry[1];
                hash[index++] = word.charEntry[0];
            }
            print_hex_msg(hash, 32);
        } else {
            printf("Invalid Entry in the input file!!\n");
        }
        fclose(fp);
    } else {
        printf("Failed to open the file\n");
    }
}

int GetContentFromFile(const char* pFileName, UINT8* pBuffer, UINT32* pReadCount) {
    int lRes = 0;
    UINT32 uFileSize = 0;
    UINT32 uReadSize = 0;
    FILE* pFile = fopen(pFileName, "rb");

    if (pFile == NULL) {
        printf("%s: Failed to open file [%s]!\n", __FUNCTION__, pFileName);
        lRes = -1;
    } else {
        //Get the File Size
        fseek(pFile, 0, SEEK_END);
        uFileSize = ftell(pFile);
        fseek(pFile, 0, SEEK_SET);

        uReadSize = fread(pBuffer, 1, uFileSize, pFile);
        fclose(pFile);
        if(uReadSize != uFileSize) {
            printf("%s: Failed to Read From file [%s]!\n", __FUNCTION__, pFileName);
            lRes = -1;
        } else {
            //print_out(pBuffer, uReadSize);
            *pReadCount = uReadSize;
        }
    }
    return lRes;
}

void get_hex_string(const char* fileName) {
    UINT32 size = 0;
    UINT8 string[1024];
    UINT8 out_string[2 * 1024];
    GetContentFromFile(fileName, string, &size);
    unsigned int index = 0;
    unsigned int idx = 0;
    unsigned int val = 0;
    unsigned int digit = 0;

    for(; index < size ; index++) {
        //printf("0x%.2x,", string[index]);
        val = string[index];
        printf("%.2x ", val);
        digit = (val >> 4) & 0x0F;
        if (digit <= 9) {
            out_string[idx++] = 0x30 + digit;
        } else if (digit <= 0xf) {
            out_string[idx++] = 87 + digit;
        }
        digit = val & 0x0F;
        if (digit <= 9) {
            out_string[idx++] = 0x30 + digit;
        } else if (digit <= 0xf) {
            out_string[idx++] = 87 + digit;
        }
        if ((index & 0xF) == 0xF) {
            printf("\n");
            out_string[idx++] = '\n';
        } else {
            out_string[idx++] = ' ';
        }
    }
    out_string[idx++] = '\0';
    printf("string format (%d): \n%s\n", idx, out_string);
}


int main () {

    const char* fName = "Girish";
    const char* lName = "Shetty";
    char key_array[4];
    const unsigned char* pkey = sam;
    char sample [100];
    int j = 0;

    //get_hex_string("/home/gshetty/pgms/HDCP2x/HDCPKeyGenerator/keys/z2_keys/HDCP20RX_dn.store");
    get_hex_string("/home/gshetty/pgms/HDCP2x/HDCPKeyGenerator/keys/z2_keys/HDCP20RX_Priv.store");
    //get_hex_string("/home/gshetty/pgms/HDCP2x/HDCPKeyGenerator/keys/z2_keys/HDCP20RX_LC.store");
    //readHash();

#if 0
    pointerTest();


    printf("%s len is : %d\n", fName, strlen(fName));

    debug_printf("Girish\n");
    DEBUG_PRINTF("%s's last name is %s\n", fName, lName);

    DEBUG_PRINTF("AES_ENCRYPT is : %d\n", AES_ENCRYPT);
    DEBUG_PRINTF("AES_DECRYPT is : %d\n", AES_DECRYPT);

    debug_printf("====================================\n");
    debug_printf("\nKey is : ");
    for(; j<sizeof(sam);j+=2) {
        memcpy(key_array, &pkey[j], 2);
        key_array[2] = ' ';
        key_array[3] = '\0';
        debug_printf(key_array);
    }

    divisibleBy4(1);
    divisibleBy4(2);
    divisibleBy4(3);
    divisibleBy4(4);
    divisibleBy4(7);
    divisibleBy4(9);
    divisibleBy4(50);
    divisibleBy4(52);
    divisibleBy4(100);
    divisibleBy4(1022);
    divisibleBy4(1442);
    divisibleBy4(1444);
    divisibleBy4(24);
#endif
}

