/** 
 *  @file ProducerConsumer.c
 *  Part of     : Open C / ProducerConsumer
 *
 *  Description : ProducerConsumer Problem solution
 *  Version:   0.1
 *
 *  Copyright (c) 2007 Nokia Corporation.
 *  This material, including documentation and any related
 *  computer programs, is protected by copyright controlled by
 *  Nokia Corporation.
 *
 */


/* INCLUDE FILES */
#include <pthread.h>
#include <errno.h>
//#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
//#include <sys/ioctl.h>
#include <stdio.h>
#include <stdlib.h>
#include "CommanHeader.h"

int main() 
{
	ThreadParam threadParams[MAX_CLIENTS + 1];
	pthread_t threadID[MAX_CLIENTS + 1];
	int i = 0;
	int noOfConsumers = 0;
	int noOfItems = 0;
	int totalItems = 0;
	int ret = 0;
	int exitReason = 0;
	sem_t semLock;
	pthread_attr_t threadAttr;

	if( sem_init( &semLock, 0, 1 ) != 0 ) 
	{
		printf("Error Creating semaphore");
		exit(-1);
	}

	/* Read all the inputs needed */
	printf("Enter Number of Consumers : ");
	scanf("%d", &noOfConsumers);

	if(MAX_CLIENTS < noOfConsumers) 
	{
		noOfConsumers = MAX_CLIENTS;
	}

	for(i=1; i<=noOfConsumers; i++) 
	{
		printf("Items Needed by Consumer-%d : ", i);
		scanf("%d", &noOfItems);
		totalItems += noOfItems;

		threadParams[i].noOfItems = noOfItems;
		threadParams[i].itemLock = semLock;
		threadParams[i].noOfConsumers = i;
	}

	/* For the Producer	 */
	threadParams[0].noOfItems = totalItems;
	threadParams[0].noOfConsumers = noOfConsumers;
	threadParams[0].itemLock = semLock;

	pthread_attr_init( &threadAttr );
	pthread_attr_setdetachstate( &threadAttr, PTHREAD_CREATE_JOINABLE );
	
	/* Create a Producer thread now */
	ret = pthread_create( &threadID[0], &threadAttr, ProducerThreadEntryPoint, 
		(void*)&threadParams[0] );

	if(ret != 0) 
	{
		printf("Error Creating Producer Thread");
		exit(-1);
	}

	/* Create a Consumer Threads now */
	for(i=1; i <= noOfConsumers; i++) 
	{
		ret = pthread_create( &threadID[i], &threadAttr, ConsumerThreadEntryPoint, 
			(void*)&threadParams[i] );

		if(ret != 0) 
		{
			printf("Error Creating Consumer Thread %d", i);
			exit(-1);
		}
	}

	/* Call C++ function that creats Observer Thread */
	CreateObserverThread (totalItems * 2);

	/* Wait for the completion of all the threads */
	for(i=0; i<=noOfConsumers; i++) 
	{
		ret = pthread_join(threadID[i], (void**)&exitReason );
	}

	/* Destroy the semaphore */
	sem_destroy( &semLock );
	printf("Completed the Production/Consumption..\n Press Any Key to Exit");
	getchar();
	getchar();
	return 0;
}

/*  End of File */
