#include <stdio.h>
int main() {
	FILE* fp = NULL;
	printf("BEGIN");
	fp = freopen("file.txt", "w", stdout);
	fclose(fp);
	
	printf("END");
	return 0;
}
