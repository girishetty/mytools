/** 
 *  @file MsgQClient.c
 *  Description: A multithread program used for Testing MsgQLib library.
 *  This application will create 2 message queues and creates 2 threads
 *  passing them the queue id. These threads will send and receive messages
 *  to each other.
 *  Copyright (c) 2007 Nokia Corporation.
 *  This material, including documentation and any related
 *  computer programs, is protected by copyright controlled by
 *  Nokia Corporation.
 */


# include <pthread.h>
# include <stdio.h>
# include <string.h>
# include <unistd.h>

int q1 = 1;
int q2 = 2;

/**
 Entry point for thread one
 This will send message to q1 and wait for some messages from q2
*/
void ThreadOne(int *id) {
	int i=1;
	int err = 0;
	int result = 0;
	int pri = 0, timeout = 0;
	int len = 100;
	int nBytes;
	char smsg[] =  "Thread#1:-> Sending Some Data through MsgQ";
	char rmsg[100];

	printf("Thr1: Strated thread : %d\n", *id);

	/* Try to Create q1 again, this will create the queue again,
	   this will just return as its already created by main thread */
	//result=MsgQCreate(q1 ,10,1,&err);
	printf("Thr1: Q CREATE result = %d\n",result);

	while(i>=0) {
		nBytes = strlen(smsg); 
		/* Send Message to q1 */
		//result = MsgQSend(q1, smsg, nBytes, pri, timeout, &err); 
		printf("Thr1: Q SEND result = %d\n",result);

		sleep(1);
		rmsg[0] = '\0';
		/* Receive Message from q2 */
		//result = MsgQReceive(q2, rmsg, len, timeout, &err); 
		rmsg[result] = '\0';
		printf("Thr1: Q RECEIVE result = %d\n", result);
		printf("Thr1: Message Receicved from Message Queue 2 is : %s\n",rmsg);
		i--;
	}
	sleep(2);
	/* delete message q1 */
	//result=MsgQDelete(q1, &err);
	printf("Thr1: Q DELETE result = %d\n",result);
}

/**
 Entry point for thread one
 This will send message to q1 and wait for some messages from q2
*/
void ThreadTwo(int *id) {
	int i=1;
	int err = 0;
	int result = 0;
	int pri = 0;
	int timeout = 0;
	int len = 100;
	int nBytes;
	char smsg[ ] = "Thread#2:-> Sending Some Data through MsgQ";
	char rmsg[100];

	printf("Thr2: Strated thread : %d\n", *id);

	while(i>=0) {
		nBytes = strlen(smsg);
		/* Send Message to q2 */
		//result = MsgQSend(q2, smsg, nBytes, pri, timeout, &err); 
		printf("Thr2: Q SEND result = %d\n",result);

		sleep(1);
		rmsg[0] = '\0';
		/* Receive Message from q1 */
		//result = MsgQReceive(q1, rmsg, len, timeout, &err); 
		rmsg[result] = '\0';
		printf("Thr2: Q RECEIVE result = %d\n",result);
		printf("Thr2: Message Receicved from Message Queue 1 is : %s\n",rmsg);
		i--;
	}
	
	sleep(2);
	/* delete message q2 */
	//result=MsgQDelete(q2,&err);
	printf("Thr2: Q DELETE result = %d\n",result);
}

int main() {
	int result, err;
	pthread_t  thread1, thread2;

	printf("Entered Main() \n");

	/* Create 2 Message Queues */
	//result = MsgQCreate(q1 ,10, MSG_Q_FIFO, &err);
	printf("Q CREATE result = %d \n",result);
	printf("Q CREATE err    = %d \n",err);

	//result=MsgQCreate(q2 ,10,MSG_Q_FIFO,&err);
	printf("Q CREATE result = %d \n",result);
	printf("Q CREATE err    = %d \n",err);

	/* Create 2 Threads */
	if(pthread_create(&thread1, NULL, ThreadOne, (void*) &q1) != 0) {
		printf("Failed to create the thread \n");
	}

	if(pthread_create(&thread2, NULL, ThreadTwo, (void*) &q2) != 0) {
		printf("Failed to create the thread \n");
	}

	/* Wait for the threads to complete */
	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);
	printf("Exiting from Main() \nEnter a Key to Exit");
	getchar();
	return 0;
}

