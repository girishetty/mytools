//#include <stdio.h>
#define CUBE(x) ((x) * (x) * (x))

int main() {
	int a = 5;
	int b = 5;
	printf("%d \n", CUBE(++a));
        printf("%d \n", (++b * ++b * ++b) );
	return 0;
}

