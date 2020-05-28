int main(){
	int a = 20;
	const int b = 40;
	int c =  100;
	int* p = &b;
	int array[b];
	
	printf("Address of a is %p  Value is %d\n", &a, a);
	printf("Address of b is %p  Value is %d\n", &b, b);
	printf("Address of c is %p  Value is %d\n", &c, c);
	printf("p points to %p  Value of p is %d\n", p, *p);

	*p = 500;

	printf("\n\nAddress of b is %p  Value is %d\n", &b, b);
	printf("p points to %p  Value of p is %d\n", p, *p);

	return 0;
}

