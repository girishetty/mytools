#include <stdio.h>
#include <stdlib.h>

int globVal = 20;

#if 0
extern void eprintf(const char *format, ...);
extern void exitnow();
extern int add_up_to_n(int n);
#else
extern void eprintf(const char *format, ...) __attribute__((format(printf, 1, 2)));
extern void exitnow() __attribute__((noreturn));
extern int add_up_to_n(int n) __attribute__((const));
#endif

void eprintf(const char *format, ...) {
}
void exitnow() {
	printf("Application Exiting now\n");
	exit(0);
}
int add_up_to_n(int n){
	int tot = n--;
	for (; n>0; n-- ) {
		tot += n;
	}
	return tot;
}

int attribute_test(int n) {
	int i = 1;
	int val = 0;
	char* ptr = "Hi";
	/* usage of attribute format */
#if 0
	eprintf("s=%s\n", 5);             /* error on this line */
#else
	eprintf("s=%s\n", ptr);           /* no error on this line */
#endif

#if 0
	eprintf("n=%d,%d,%d\n", 1, 2);    /* error on this line */
#else
	eprintf("n=%d,%d,%d\n", 1, 2, 3); /* no error on this line */
#endif

	/* usage of attribute const */
	for(;i<10;i++) {
		val = val * i + add_up_to_n(10);
	}

	/* usage of attribute noreturn */
	if(n<0)
		exitnow();
	else
		return 0;
}

int main() {
	attribute_test(6);
	return 0;
}

