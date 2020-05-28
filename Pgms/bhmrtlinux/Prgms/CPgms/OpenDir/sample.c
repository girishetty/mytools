#include <stdio.h>
#include <fcntl.h>
#include <errno.h>

typedef int int_array[10];

int main() {
	//char string[20];
	//int fd = -9078787;
	int err = 0;
	int fd = open("/home/girishs", O_RDONLY);
	char buff[100];
	if(fd != -1) {
		err = read(fd, buff, 100);
		perror("read error");
		printf("Error no is %d err is %d\n", errno, err);
	}
	
	//int fd = open("/home/girishs", O_WRONLY);
	//int ret = read(fd, string, 10);
	//printf("ret is %d error is %d, %s", ret, errno, strerr(errno));
	else {
		perror("open error");
		printf("Error no is %d\n", errno);
	}
	close(fd);
	return 0;
	}

