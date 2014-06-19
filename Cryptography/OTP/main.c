#include <stdio.h>

void print_details(const void* pv, size_t len)
{
    const unsigned char * p = (const unsigned char*)pv;
    if (NULL == pv)
        printf("NULL");
    else
    {
        size_t i = 0;
        for (; i<len;++i)
            printf("%02X ", *p++);
    }
    printf("\n");
}

int main() {
    char plainText1 [] = "attack at dawn";
    char cipherText1 [] = { 0x09, 0xe1, 0xc5, 0xf7, 0x0a, 0x65, 0xac, 0x51, 0x94, 0x58, 0xe7, 0xe5, 0x3f, 0x36 };
    //char cipherText1 [] = { 0x6c, 0x73, 0xd5, 0x24, 0x0a, 0x94, 0x8c, 0x86, 0x98, 0x1b, 0xc2, 0x94, 0x81, 0x4d };
    char plainText2 [] = "attack at dusk";
    char cipherText [15];
    char key[14];
    int i = 0;

    for(i=0;i<14;i++) {
        key[i] = cipherText1[i] ^ plainText1[i];
    }

    for(i=0;i<14;i++) {
        cipherText[i] = plainText1[i] ^ key[i];
    }
    cipherText[14] = '\0';
    printf("PlainText (%s)   CipherText (%s) \n", plainText1, cipherText);
    print_details(cipherText, 14);

    for(i=0;i<14;i++) {
        plainText1[i] = cipherText[i] ^ key[i];
    }
    printf("PlainText (%s)   CipherText (%s) \n", plainText1, cipherText);

    for(i=0;i<14;i++) {
        cipherText[i] = plainText2[i] ^ key[i];
    }
    cipherText[14] = '\0';
    printf("PlainText (%s)   CipherText (%s) \n", plainText2, cipherText);
    print_details(cipherText, 14);

    for(i=0;i<14;i++) {
        plainText2[i] = cipherText[i] ^ key[i];
    }
    printf("PlainText (%s)   CipherText (%s) \n", plainText2, cipherText);
    return 0;
}
 
