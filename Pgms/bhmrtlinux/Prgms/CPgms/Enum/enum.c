enum MyEnum {
	Val1,
	Val2,
	Val3
};

int main() {
	enum MyEnum e;
	e = Val2;

	printf("Val1 is %d  e is %d\n", Val1, e);
	e = Val3;

	printf("Val1 is %d  e is %d\n", Val1, e);
	e = 78978775678908;
	printf("Val1 is %d  e is %d\n", Val1, e);
	return 0;
}

