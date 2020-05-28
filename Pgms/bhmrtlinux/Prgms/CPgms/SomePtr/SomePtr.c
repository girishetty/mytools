
void OpTest() {
	int array [] = {10, 9 , 8, 7 };
	int* p1 = array;
	int* p2 = array;
	int* p3 = array;
	int* p4 = array;
	int val;

	val = *p1++;
	printf("Val is : %d  *p1 is %d  p1 is %x\n", val, *p1, p1);
	val = *(p2++);
	printf("Val is : %d  *p2 is %d  p2 is %x\n", val, *p2, p2);
	val = ++(*p3);
	//val = ++*p3;
	printf("Val is : %d  *p3 is %d  p3 is %x\n", val, *p3, p3);
	//val = ++(*p4);
	val = ++*p4;
	printf("Val is : %d  *p4 is %d  p4 is %x\n", val, *p4, p4);
}

void CharIntPtrTest() {
	char* cPtr;
	char* cPtr1;
	int array [] = {1, 2 , 3, 4 };
	int val;

	cPtr = (char*) array;
	cPtr1 = (char*) array;
	val = (int) *cPtr;
	printf("Value is : %d\n", val);
	
	cPtr += sizeof(int);
	//cPtr1 = ((int *)cPtr1 + 1);
	val = (int) *cPtr;
	printf("Value is : %d\n", val);
	val = (int) *cPtr1;
	printf("Value is : %d\n", val);
}

int main() {
	//CharIntPtrTest();
	OpTest();

	return 0;
}

