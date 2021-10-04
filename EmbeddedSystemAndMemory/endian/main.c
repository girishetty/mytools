#include <stdio.h>


typedef union {
    unsigned int  word;
    unsigned char byte[4];
} WORD;

int main () {

    WORD w;
    w.word = 0x01020304;

    if (w.byte [0] == 0x01) {              //MSB is stored first
        printf("BIG ENDIAN SYSTEM\n");
    } else if (w.byte [0] == 0x04) {       //LSB is stored first
        printf("LITTLE ENDIAN SYSTEM\n");

    }

    int x = 0x01;
    char* y = (char*)&x;
    printf("%c\n", *y + 48);

    x = 0x0100;
    printf("%c\n", *y + 48);

    return 0;
}

