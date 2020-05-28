#include<stdio.h>
#include<string.h>
#include<locale.h>

int main()
	{
	const char* strlit3 = "AUGMENT";
	int len = strxfrm(NULL,strlit3, 0);
	char pstrdst[100] = {0};
	char* ret = setlocale(LC_ALL,"C");
	printf("ret is : %s\n", ret);
	printf("len is : %d\n", len);
	len = strxfrm(NULL,strlit3,0);
	len = strxfrm(pstrdst,strlit3, strlen(strlit3));
	pstrdst[len] = '\0';
	printf("len is : %d\n", len);
	printf("str is : %s\n", pstrdst);
	getchar();
	return 0;
	}

