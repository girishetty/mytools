#include <stdio.h>
#include <unistd.h>

int main() {
	int i = 0;
	fprintf(stdout, "Hello world\n");

	while(i++<20) {
		fprintf(stdout, "STDOUT ");
		fprintf(stderr, "STDERR ");
		sleep(1);
	}
	fprintf(stdout, " After WHILE ");
	fprintf(stderr, " After WHILE ");
	return 0;
}

