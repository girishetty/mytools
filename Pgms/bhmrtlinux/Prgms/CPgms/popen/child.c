#include <stdio.h>

int main(int argc, char** argv) { 
	printf("Child Process...\n");
        int fd; 
        scanf("%d", &fd); 
        
        write(fd, "Hello", 5); 
        close(fd); 
	printf("Child Exits..\n");
        return 0; 
} 

