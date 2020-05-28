#include <stdio.h>
#include <errno.h>

int ReadWriteFile(char* file, char* wmode, char* rmode) {
	FILE* fp1 = NULL;
	FILE* fp = NULL;
	char* writeData1 = "Line_Number_One_Data";
	char* writeData2 = "Line_Number_Two_Data";
	char* writeData3 = "Line_Number_Three_Data";
	char str[50];
	fp = fopen(file, wmode);
	if(fp)
		{
		fprintf(fp, "%s", writeData1);
		fprintf(fp, "%s", writeData2);
		fprintf(fp, "%s", writeData3);
		fclose(fp);
		printf("Wrote Data\n");
		}
	else
		{
		printf("fopen failed with %d\n", errno);
		return 0;
		}

	fp1 = fopen(file, rmode);
	if(fp1)
		{
		fscanf(fp1, "%s", &str);
		printf("Read Data : %s\n", str);
		str[0] = '\0';

		fscanf(fp1, "%s", &str);
		printf("Read Data : %s\n", str);
		str[0] = '\0';

		fscanf(fp1, "%s", &str);
		printf("Read Data : %s\n", str);
		fclose(fp1);
		printf("Read Data Complete\n");
		}
	return 0;
	}

int main() {
	printf("Binary File I/O\n");
	/*ReadWriteFile("myfile1.txt", "wb", "rb"); */
	printf("Text File I/O\n");
	ReadWriteFile("myfile2.txt", "w", "r");
	return 0;
}

