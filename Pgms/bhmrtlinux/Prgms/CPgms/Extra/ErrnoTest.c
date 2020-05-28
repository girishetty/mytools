#include <stdio.h>
#include <errno.h>
#include <fcntl.h>

int main() {
      int fd = 0;
      /* Assuming file name specified does not exists! */
      FILE* fp = fopen("does_not_exist1.txt", "r");
      perror("Status of fopen is : ");
      if(fp) {
         printf("File Opened in Read Mode, error code is %d\n", errno);
         fclose(fp);
      }
      else {
         printf("File Open Failed with %d\n", errno);
         errno = 0;
      }
      /* Try to create the same file in write mode now */
      fd = open("does_not_exist1.txt", O_WRONLY | O_CREAT);
      perror("Status of open is : ");
      
      if(fd == -1) {
         printf("File create in write mode Failed with %d\n", errno);
      }
      else {
         printf("File Created for write, error code : %d\n", errno);
         errno = 0;
      }
      close(fd);
      return 0;
}


