#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

//forward declarations.
void display_stat(struct stat* buf);
int read_dir_opern(int fd);

int dir_opern() 
	{
	char* dir = "/home/girishs/";
	int mode = O_RDONLY; // O_WRONLY 
	int fd = open(dir, mode);
	if( fd > 0 )
		{
		//Read from the directory
		read_dir_opern( fd );
		int offset = 30;
		int where = /*SEEK_CUR;*/ SEEK_END; //SEEK_SET
		errno = 0;
		int res = lseek(fd, offset, where);
		printf("lseek returned with %d error : %d\n", res, errno);
		struct stat buf;
		memset(&buf, 0, sizeof(buf));
		res = fstat(fd, &buf);	
		printf("fstat returned with %d error : %d\n", res, errno);
		display_stat(&buf);
		close(fd);
		fd = 0;
		}
	else
		{
		printf("Directory open failed with %d\n", errno);
		}
	return fd;
	}

int read_dir_opern(int fd)
	{
	int i = 0;
	char buff[200];
	char* file = "/home/girishs/dircontents.txt";
	int retVal = read(fd, buff, 200);
	if( retVal > 0 )
		{
		FILE* fp  = fopen(file, "w");
		if( fp )
			{
			for(; i<retVal; i++)
				{
				fprintf(fp, "%c", buff[i]);
				printf("%c", buff[i]);
				}
			fclose(fp);
			}
		else
			{
			printf("log file open returned with %d\n", errno);
			}
		}
	else
		{
		printf("there is nothing to read from this directory\n");
		}
	return retVal;
	}

void display_stat(struct stat* buf)
	{
	printf("device is %d\n", buf->st_dev);
	printf("mode is %d\n", buf->st_mode);
	printf("size is %d\n", buf->st_size);
	}

int main () {
	dir_opern();
	return 0;
}

