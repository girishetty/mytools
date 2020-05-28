#include <stdio.h>

char *c[] ={ "FILE", "EDIT", "SEARCH", 	"COMPILE" };
char **cp[] = {c+3, c+2, c+1, c};
char ***cpp = cp;

int main() {
	printf("%s", **cpp);
	printf(" %s", **++cpp);
	printf(" %s", *(cpp[3]-5));
	printf(" %s\n", cpp[-1][-2]);
	return 0;
}

