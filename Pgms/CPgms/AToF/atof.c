#include <stdio.h>

int main() {

	struct atoi_string
		{
		char* float_string;
		float float_val;
		};

	struct atoi_string string[3] =
		{
		"-0.76786", -0.767860,
		"-75478.987987", -75478.987987,
		"-67.78", -67.780000
		};

	int i=0;
	float ret; 
	int val;
	for(i=0;i<3;i++)
		{
		ret = atof(string[i].float_string);
		val = atof(string[i].float_string);
		printf("atof returned : %f\n", ret);
		printf("expected value : %f\n", string[i].float_val);
		if(string[i].float_val != ret)
			{
			printf("Failed !!\n");
			}
		}

	return 0;
}