#include <stdio.h>

#define MACRO(a) a+10
#define MY_MACRO(a) MACRO(a)+5
#define MYMACRO(a) MY_MACRO(a*2)

void my_foo()
{

}

void my_foo1(void)
{

}

void foo(int a, int b)
{
}

int main() {
	printf("%d\n", MYMACRO(10));
	foo(3, 4, 5);
	//my_foo(3, 4, 5);
	my_foo1(3, 4, 5);
	return 0;
}
