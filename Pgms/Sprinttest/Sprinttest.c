#if 0
#include <stdio.h>

int main() {
	unsigned __int64 val = 16889980564398564531;
	//unsigned __int64 val = 473588231;
	
	char str1[50];
	char str2[50];
	char str3[50];
	sprintf(str1, "%I64u", val);
	sprintf(str2, "%lu", val);
	sprintf(str3, "%llu", val);
	printf("val is : %I64u\n", val);
	printf("val is : %llu\n", val);
	printf("str1 is : %s\n", str1);
	printf("str2 is : %s\n", str2);
	printf("str3 is : %s\n", str3);
	getchar();
	return 0;
}

#endif
