#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int main() {
	int ret = 0;
	struct stat buf1, buf2;
	char* file = "sample.txt";
	int fp = open(file, O_WRONLY|O_CREAT);
	if(fp != -1) {
	 	ret = fstat(fp, &buf1);
		write(fp, "hi", 2);
	 	ret = fstat(fp, &buf2);
		printf("%d  %d\n", buf1.st_size, buf2.st_size);
		fsync(fp);
	 	ret = fstat(fp, &buf2);
		printf("%d\n", buf2.st_size);
		close(fp);
	}
	return 0;
}

