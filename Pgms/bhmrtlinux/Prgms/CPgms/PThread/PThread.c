#include <stdio.h>
#include <pthread.h>

void* ThreadEntryPoint(void* aParam)
	{
	int retVal = 0;
	printf("New Thread Started Execution..\n");
	pthread_t selfID = pthread_self();
	printf("New Thread Id is : %u\n", selfID);
	if( selfID == 0 )
		{
		retVal = -1;
		pthread_exit( (void*) retVal );
		}
		
	if( (int)aParam != 0 )
		{
		//Exit the thread with the reason
		pthread_exit( aParam );
		}
		
	printf("New Thread Terminating..\n");
	return (void*)retVal;
	}

int main() {
	int retVal;
	pthread_t threadID1 = 0;
	pthread_t threadID2 = 0;
	pthread_attr_t threadAttr;
	pthread_attr_init( &threadAttr );
	pthread_attr_setdetachstate( &threadAttr, PTHREAD_CREATE_JOINABLE);		
	printf("Thread Id of Main Thread is : %u\n", pthread_self());
	//Create a thread now
	retVal = pthread_create( &threadID1, &threadAttr, ThreadEntryPoint, (void*)0);
	printf("pthread_create return value is : %d\n", retVal);
	if( retVal == 0) {
		retVal = pthread_detach( threadID1 );
		printf("pthread_detach return value is : %d\n", retVal);
		if (retVal == 0) {
			//Create one more thread
			retVal = pthread_create( &threadID2, &threadAttr, ThreadEntryPoint, (void*)0);
			printf("pthread_create return value is : %d\n", retVal);
			if (retVal == 0) {
				int threadRet;
				retVal = pthread_join( threadID2, (void**)&threadRet);
				printf("pthread_join return value is : %d\n", retVal);
			}
		}
	}
	printf("Thread Id of Main Thread is : %u\n", pthread_self());

	printf("Before pthread_exit\n");
	pthread_exit((void*)0);
	printf("After pthread_exit\n");

	return 0;
}

