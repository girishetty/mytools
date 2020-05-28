#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main() {
	int fd, newfd, ret, getfd = 15;
	char* data = "write me\0";
	const int size = 9;

	fd = open("file1.txt", O_WRONLY | O_CREAT);
	printf("file open ret is : %d\n", fd);

	newfd = dup2(fd, getfd);
	printf("dup2 ret is : %d\n", newfd);

	ret = write(getfd, data, size);
	printf("ret is : %d\n", ret);

	ret = write(newfd, data, size);
	printf("ret is : %d\n", ret);

	close(newfd);
	close(fd);
	}

