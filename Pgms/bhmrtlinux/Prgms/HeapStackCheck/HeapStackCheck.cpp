#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void StackFrameCheck(int i) {
	int j = 0;
	int* addr = 0;
	printf("\nIn StackFrameCheck\n");
	printf("&i: %08x\n&StackFrameCheck: %08x\n", (int)(&i), (int)(StackFrameCheck));
	for (j=0; j<4; j++) {
		addr = (int*)(&j+j);
		printf("Address: %08x   Content: %08x\n", addr, *addr);
	}
}

void PirateFunc() {
	printf("Stack overflow!\n");
}

void StackPtrCheck(int i) {
	int j[2];
	j[3] = (int)(PirateFunc);
	return;
}

void GoTest() {
	StackPtrCheck(1);
}

void DisplayHeader(int* ptr1, int* ptr2, int* ptr3, int headerSize) {
    printf("\nInside DisplayHeader\n");  
	for(int i=0; i<headerSize;i++) {
                printf("%08x    %08x    %08x\n", *ptr1, *ptr2, *ptr3);
                ptr1++;
                ptr2++;
                ptr3++;
        }
}

void HeapAllocationCheck() {
        char *buf1, *buf2, *buf3, *buf4, *buf5;
        printf("\nIn HeapAllocationCheck\n");
        buf1 = (char*)malloc(4096);
        buf2 = (char*)malloc(8192);
        buf3 = (char*)malloc(4096);
        buf4 = (char*)malloc(4096);

        printf("buf1 : %08x\n", buf1);
        printf("buf2 : %08x\n", buf2);
        printf("buf3 : %08x\n", buf3);
        printf("buf4 : %08x\n", buf4);

        int headerSize = (int) buf2 - (int) buf1 - 4096;
        printf("Heap Header Size is : %d Bytes\n", headerSize);

        char* ptr = buf1 + 4096;
        int* ptr1 = (int*) ptr;
        ptr = buf2 + 8192;
        int* ptr2 = (int*) ptr;
        ptr = buf3 + 4096;
        int* ptr3 = (int*) ptr;

        //Display the Header Info
	headerSize /= 4;
        DisplayHeader(ptr1, ptr2, ptr3, headerSize);

        //Corrupt first 8 bytes of the header Info
        
	*ptr1 = 0x4444; // this will lead to CRASH while doing free, as header info is changed now
        //*(ptr1 + 1) = 0xAAAA; //this wont CRASH

        DisplayHeader(ptr1, ptr2, ptr3, headerSize);
        //Copy some data to the 2nd buffer
        memcpy(buf2, "Hello Symbian", 12);

        DisplayHeader(ptr1, ptr2, ptr3, headerSize);

        //allocate new heap cell
        buf5 = (char*) malloc(4096);

        DisplayHeader(ptr1, ptr2, ptr3, headerSize);

        //free all heap buffers now
        free(buf1);
        free(buf2);
        free(buf3);
        free(buf4);
        free(buf5);

        //Display header info now.. headers will change now, as memory is released
	DisplayHeader(ptr1, ptr2, ptr3, headerSize);

        printf("returning from HeapAllocationCheck\n");
}

void MallocTest() {
	char *buf1, *buf2;
	buf1 = (char*)malloc(4096);
	//*(int*)(buf1+4096+4)=3;
	buf2 = (char*)malloc(4096);
	*(int*)(buf1+4096+4)=3;
}

int main() {
	int i = 0x12344322;
	printf("In Main : Address of Main is : %08x\n", &main);
	printf("i is : %08x and Address of i is : %08x\n", i, &i);
	//StackFrameCheck(i);
	//HeapAllocationCheck();
	//GoTest();
	MallocTest();
	return 0;
}


