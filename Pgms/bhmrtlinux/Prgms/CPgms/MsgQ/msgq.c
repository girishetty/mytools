#include <pthread.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#define __USE_GNU 1
#include <sys/msg.h>

#define KMSGQKEY 	1000
#define KMAXSENDMSG  	100


int ThreadEntryPoint(void* ptr) {
	int retVal = 0;
	struct msgbuf* recvMsg = NULL;
	key_t msgQFd = msgget(1000, IPC_CREAT);
	if (msgQFd == -1) {
    		printf("Msg Q Creation Failed : %d\n", errno);
    		return -1;
    	}

	recvMsg = (struct msgbuf*)malloc(KMAXSENDMSG);
	retVal = msgrcv(msgQFd, recvMsg, KMAXSENDMSG, 0, 0);
	if (retVal > 0) {
		printf("Received Msg : %s\n", recvMsg->mtext);
    		return -1;
    	}

	retVal = msgctl(msgQFd, IPC_RMID, NULL);
	free(recvMsg);
	
	return 0;
}

int main() {
	pthread_t threadID;
	pthread_attr_t threadAttr;
	int ret = 0;
	int exitReason = 0;
	struct msgbuf* sendMsg = NULL;
	key_t msgQFd = msgget(1000, IPC_CREAT);

	if (msgQFd == -1) {
    		printf("Msg Q Creation Failed : %d\n", errno);
    		return -1;
    	}

	sendMsg = (struct msgbuf*)malloc(KMAXSENDMSG);
	sendMsg->mtype = 1;
	sprintf(sendMsg->mtext, "This is the Data to be Sent");
	ret = msgsnd(msgQFd, sendMsg, strlen(sendMsg->mtext), 0);
	if (ret == -1) {
		printf("Send Msg Failed with : %d\n", errno);
    		return -1;
    	}

	pthread_attr_init( &threadAttr );
	pthread_attr_setdetachstate( &threadAttr, PTHREAD_CREATE_JOINABLE );

	ret = pthread_create( &threadID, &threadAttr, ThreadEntryPoint, (void*)NULL );

	if(ret != 0) {
		printf("Error Creating Thread\n");
		return -1;
	}

	ret = pthread_join(threadID, (void**)&exitReason );
	ret = msgctl(msgQFd, IPC_RMID, NULL);
	free(sendMsg);

	return 0;
}

