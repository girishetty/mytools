#include <stdio.h>
#include <math.h>

#define MAX 100
#define M 8
#define N 6
#define GAP (M-N)

void encode(char inp[MAX], int n, char res[MAX]);
void mdecode(char res[MAX], int n, char decode[MAX]);

int main()
{
	//char input[MAX] = {0};
	char res[MAX] 	= {0};
	char decode[MAX]= {0};
	char input[8] = {0x56,0x42,0x7A, 0x25, 0x01, 0x36, 0x69, 0x3A};
	int n = 8;
	int i;
	/*
	scanf("%d",&n);
	for(i=0;i <n ; i++)
	{
		scanf("%d", &input[i]);

	}*/
	encode(input,n, res);
	printf("After Encoding......\n");
	for(	 i=0;i <n ; i++)
	{
		printf("%x\n", res[i]);
	}

	mdecode(res,n, decode);
	printf("After Decoding......\n");
	for(	 i=0;i <n ; i++)
	{
		printf("%x \n", decode[i]);
	}


	return 0;
}


void mdecode(char res[MAX], int n, char decode[MAX])
{
	int i;
	char mask;
	char residue[MAX];
	for( i=0 ; i<n ; i++)
	{
		mask 	     = pow(2,i+GAP)-1;
		residue[i]   = res[i]&mask;
		residue[i]   = residue[i]<<(8-(i+1-1)); /*1 as a significance*/
	}

	for( i=0 ; i<n ; i++)
	{
	if(!i)
	   decode[i] = res[i]>>(i+1);
	else
	 {
		decode[i] = res[i]>>(i+1);
		decode[i] = decode[i] | residue[i-1];
	 }


	}


}




void encode(char inp[MAX], int n, char res[MAX])
{
	int i;
	char tmp;
	char mask;

	for(i =0 ; i<n ; i++)
	{
		res[i]  = inp[i]<< (i+1);
		if(i==n-1) break;
		mask    = pow(2,i+1)-1;
		mask    = mask << (8-(i+1));
		tmp	= mask&inp[i+1];
		tmp	= tmp >> (8- (i+1));
		res[i]  = res[i] | tmp;
	}


}
