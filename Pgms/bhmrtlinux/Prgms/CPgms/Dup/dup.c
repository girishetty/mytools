#include <stdio.h>
#include <fcntl.h>
#include <errno.h>

void Dup2Test() 
	{
	const char* file = "file.txt";
	int fd, newFd = 1000;
	int i=0, err;
	fd = open(file, O_WRONLY);
	if(fd < 0 )
		{
		printf("open failed with %d\n", errno);
		return;
		}
	newFd = fd;
	err = dup2(fd, newFd);
	if(err < 0 )
		{
		printf("dup2 failed with %d\n", errno);
		close(fd);
		return;
		}
	printf("sent fd (%d, %d), new fd is : %d\n", fd, newFd, err);
	close(fd);
	close(newFd);
	printf("dup2 success\n");
	}

void DupTest()
	{
	const char* file = "file.txt";
	int fd[10];
	int i=0;;
	fd[0] = open(file, O_WRONLY);
	if(fd[0] < 0 )
		{
		printf("open failed with %d\n", errno);
		return;
		}
	for(i=1; i<10; i++)
		{
		fd[i] = dup(fd[0]);
		if(fd[i] < 0)
			{
			printf("Failed for %dth attempt with error %d\n", i+1, errno);
			return;
			}
		}

	for(i=0; i<10; i++)
		{
		close(fd[i]);
		}
		
	printf("Success");
	}

int main() {
	Dup2Test();
	DupTest();
}

