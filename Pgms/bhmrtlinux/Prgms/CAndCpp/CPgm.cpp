#include <stdio.h>

int x[100];
int main () {
    struct x {
        int a;
    };
    char str[5] = "Nokia";
    printf("Size of X is : %d\n", sizeof(x));
    return 1;
}

