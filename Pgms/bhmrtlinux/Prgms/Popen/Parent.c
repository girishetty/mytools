#include <stdio.h>

int ReadFromChild() {
	int ret = 0;
	char string[100];
	FILE* fp = popen("/home/girishs/Prgms/Popen/readchild.out", "r");
	if(fp) {
		ret = fscanf(fp, "%s", string);
		printf("Read %s from child\n", string);
		pclose(fp);
	}
	return 0;
}

int WriteToChild() {
	FILE* fp = popen("/home/girishs/Prgms/Popen/writechild.out", "w");
	if(fp) {
		fprintf(fp, "PROCESS");
		/* pclose(fp); */
	}
	return 0;
}

int main() {
	ReadFromChild();
	WriteToChild();
	return 0;
}

