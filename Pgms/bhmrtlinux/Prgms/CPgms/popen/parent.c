#include <stdio.h>
#include <fcntl.h>
#include <errno.h>


int main(int argc, char** argv) {
	printf("Parent Process\n");
	 char buf[4]; 
        int fd = open("dummy.log", O_WRONLY); 
	if( fd < 0) {
		printf("dummy.log file open FAILED : errno is %d\n", errno);
		return 0;
	}
        sprintf(buf, "%d", fd); 
        
        FILE* fp = popen("./child.out", "w"); 
	if(fp) {
        	fwrite((const void*)buf, 1, 4, fp); 
        	pclose(fp); 
	}
	else
		printf("popen FAILED : errno is %d\n", errno);
	printf("Parent Exits..\n");
        return 0; 
} 

