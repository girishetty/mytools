#include <stdio.h>

enum MyEnum {
	Val1,
	Val2,
	Val3
};

/*
MyEnum operator+(MyEnum& e1, MyEnum& e2) {
	MyEnum e = MyEnum(10);
	return e;
}*/

MyEnum operator++(MyEnum& e1) {
	return MyEnum(e1 + MyEnum(1));
}

int main() {
	enum MyEnum e;
	e = Val2;

	printf("Val1 is %d  e is %d\n", Val1, e);
	e = Val3;

	printf("Val1 is %d  e is %d\n", Val1, e);
	e = MyEnum(8887666551);
	//e = 78978775678908;
	printf("Val1 is %d  e is %d\n", Val1, e);
	printf("Size of MyEnum is %d\n", sizeof(MyEnum));
	printf("MyEnum(0) is %d\n", MyEnum(5));
	e = Val3;
	printf("Val is %d\n", e + e);
	printf("Val is %d\n", ++e);
	return 0;
}

