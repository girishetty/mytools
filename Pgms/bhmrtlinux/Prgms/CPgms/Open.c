#include <stdio.h>
#include <errno.h>
#include <string.h>

int main() {
	FILE* fp = NULL;

	fp = fopen("file.txt", "rw");
	if(fp) {
		printf("Success\n");
		fclose(fp);
	}
	else
		printf("Failed with %d\n", errno);
		perror("Error is : ");
	return 0;
}

