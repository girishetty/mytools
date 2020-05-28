#include <stdio.h>

#define low_temp 0
#define high_temp 2

float degrees = 0;
volatile float* temp = &degrees;

int main()
{
   int fork_return;
   int count = 0;
   printf("Process %d about to fork a child.\n", getpid() );
   fork_return = vfork();
   if( fork_return < 0)
   {
      printf("Unable to create child process, exiting.\n");
      exit(-1);
   }
   else if( fork_return > 0)
   /* Then fork_return is the pid of the child process and I am
      the parent. Start printing a's. */
   {
        while(count++ <10)
        {
	printf("Parent:\n");
        if(*temp < low_temp)
                printf("heater is on, *temp  = %f \n", *temp);
        else if(*temp > high_temp)
                printf("cooler is on, *temp  = %f \n", *temp);
        sleep(1);
        }
   }
  else
   /* A 0 return tells me that I am the child. Print b's */
   {
        while(count++ <10)
                {
                degrees = degrees + 0.30f;
        	printf("Child : ");
                printf(" degrees = %f\n", degrees );
                sleep(1);
                }
    }
    return 0;
}


