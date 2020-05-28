#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <dirent.h>


int main() {
	int ret = 0;
#if 0
	ret = chdir("");
	printf("chdir returned with %d and errno is %d\n", ret, errno);
	errno = 0;
	chdir("123456789012345");
	printf("chdir returned with %d and errno is %d\n", ret, errno);
	errno = 0;
	chdir("chdir-t.8/chdir-t.8");
	printf("chdir returned with %d and errno is %d\n", ret, errno);
	errno = 0;
	ret = creat("./creat-t.18.1", 0777);
	printf("create returned with %d and errno is %d\n", ret, errno);
	close(ret);
	errno = 0;
	ret = creat(".abcdefghi", 0444);
	printf("create returned with %d and errno is %d\n", ret, errno);
	close(ret);
	errno = 0;
	ret = creat("123456789012345", 0777);
	printf("create returned with %d and errno is %d\n", ret, errno);
	close(ret);
	errno = 0;
	ret = creat("", 0777);
	printf("create returned with %d and errno is %d\n", ret, errno);
	close(ret);
	errno = 0;
	ret = creat("./creat-t.27/creat-t.27.1", 0777);
	printf("create returned with %d and errno is %d\n", ret, errno);
	close(ret);
	if(mkdir("enametoolong/../enametoolong/../enametoolong/../enametoolong/../enametoolong/../enametoolong/../enametoolong/../enametoolong/../enametoolong/../enametoolong/../enametoolong/../enametoolong/../enametoolong/../enametoolong/../enametoolong/././././notworking1", 0222) < 0) {

         perror(" ") ;
        }

	DIR* dir = 0;
	FILE* fp = fopen("xyz", "w");
	if(!fp) {
		printf("errno is : %d\n", errno);
		perror("Error code is : ");
	}
	else {
		fclose(fp);
	}

	errno = 0;
	dir = opendir("xyz/abc");
	if(!dir) {
		printf("errno is : %d\n", errno);
		perror("Error code is : ");
	}
	char c = 'a';
	printf("C is %c and %x\n", c, c);
	c = 'A';
	printf("C is %c and %x\n", c, c);
	c = 'z';
	printf("C is %c and %x\n", c, c);
	c = 'Z';
	printf("C is %c and %x\n", c, c);
#endif	
	FILE* fp = fopen("junk.txt", "w");
	if(fp) {
		printf("File Opened Successfully\n");
		fclose(fp);
		printf("After first close\n");
		fclose(fp);
		printf("After second close\n");
	}
	return 0;
}


