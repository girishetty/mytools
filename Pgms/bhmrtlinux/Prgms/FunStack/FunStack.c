#include <stdio.h>
#include <stdarg.h>

int myFun (int arg1, ...)
	{
	va_list ap;
	int ret = 0;
	int arg2;
	int arg3;
	
	va_start(ap, arg1);
	
	arg2 = va_arg(ap, int);
	arg3 = va_arg(ap, int);
	printf("arg1 : %d  arg2: %d  arg3: %d\n", arg1, arg2, arg3);

 	va_end(ap);
	return ret;
	}

int main() {
	int ret = 100;
	int val = 500;
	ret = myFun(10);
	printf("ret from myFun is : %d\n", ret);
	ret = myFun(10, 20);
	printf("ret from myFun is : %d\n", ret);
	ret = myFun(10, 20, 30);
	printf("ret from myFun is : %d\n", ret);
	ret = myFun(10, 20, 30, 40);
	printf("ret from myFun is : %d\n", ret);
	return 0;
	}

