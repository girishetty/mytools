# include <stdio.h>

union MyUnion {
	char data[4];
	int intVal;
	float fltVal;
};

void Display(union MyUnion val) {
	int i = 0;
	for(; i<4; i++) {
		printf("Byte %d has : %x\n", i+1, val.data[i]);
	}

	printf("Int Val is : %d\n", val.intVal);
	printf("Flt Val is : %f\n", val.fltVal);

	printf("Int in Flt Val is : %f\n", val.intVal);
	printf("Flt in Int Val is : %d\n", val.fltVal);
	getchar();
}


void MyFoo(int val) {
		printf("Val is : %d\n", val);
}

int main() {
	union MyUnion val1;
	int i = 0;

#if 0
	val1.fltVal = 10.5f;
	Display(val1);

	val1.intVal = 10;
	Display(val1);

	val1.fltVal = 10.15f;
	Display(val1);

	val1.fltVal = 00.0f;
	Display(val1);

	val1.fltVal = 20.50f;
	Display(val1);

	val1.fltVal = 00.25f;
	Display(val1);

	val1.fltVal = 100.75f;
	Display(val1);

	val1.fltVal = 120.55f;
	Display(val1);

	val1.fltVal = 700.25f;
	Display(val1);
#endif
	MyFoo(10450.5);
	return 0;
}
