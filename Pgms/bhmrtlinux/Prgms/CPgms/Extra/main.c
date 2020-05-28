#include <stdio.h>
#include <errno.h>

void Foo1() {
	int x = 50;
	int y = 20;
	x = y + x;
	printf("\nIn Foo1 : X is %d  Y is %d\n", x, y);
}

void Foo2() {
	int x = 50;
	int y = 20;
	y = y + x;
	printf("\nIn Foo2 : X is %d  Y is %d\n", x, y);
}

void Print() {
	write(1, "\nHello\n", 8);
	fprintf(stdout, "Value is %d\n", 60);
}

void Rename() {
	int ret = rename("useme", "useme.txt");
	printf("returned with %d and errno is %d\n", ret, errno);
}

int main (int argc, char *argv[]) {
	int i=0;
	/*
	if(argc > 0) 
		printf("Name of the Executable is : %s\n", argv[i]);
	for(i=1; i<argc;i++) {
		printf("Argument %d sent by User is %s\n", i, argv[i]);
	}
	Foo1();
	Foo2(); 
	Print(); */
	Rename();
	return 0;
}


