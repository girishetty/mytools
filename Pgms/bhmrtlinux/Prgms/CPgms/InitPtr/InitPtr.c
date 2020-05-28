typedef int** INT_PTR;

void Initialize(INT_PTR ptr) {
	/*
	static int dummy = 5;
	*ptr = &dummy;*/
	*ptr = (int*) malloc(sizeof(int));
	**ptr = 10;
}

int main() {
	int* myPtr = 0;
	Initialize( &myPtr );
	printf("Pointer is : %x and content is : %d\n", myPtr, *myPtr);
	return 0;
}

