#define  MSG_Q_FIFO			1
#define  MSG_Q_PRIORITY		2
#define  MSG_PRI_NORMAL		3
#define  MSG_PRI_URGENT		4
#define  SEM_Q_FIFO			5
#define  SEM_Q_PRIORITY		6

#define  OK					0
#define  ERROR				-1  

#define	 MAX_MSG_LEN		2048 
#define	 MAX_NUM_MSGS		50

typedef unsigned long ULONG;
 


class MsgQueue {
	int qId;

public:
	MsgQueue () {
	}

	int createQueue (ULONG qName, ULONG maxMsgs=MAX_NUM_MSGS, ULONG qOptions=MSG_Q_FIFO) {
		qOptions= MSG_Q_FIFO ;
		if((qId = msgget((key_t) qName ,IPC_CREAT | 0666 | IPC_EXCL )) >=0 )
			return OK;
		else
			return ERROR;
	}

	int deleteQueue () {
		if (msgctl(qId,IPC_RMID,0) == 0)
			return OK;
		else
			return ERROR;
	}

	int sendMsg(char *msg, ULONG nBytes, int timeout) {
		struct {
			long mtype;
			char mtext[nBytes];
		} message;

		message.mtype = 1;						//  FIFO Mesage Queue
		bcopy(msg, message.mtext, nBytes);     // mesage to be sent

		if(msgsnd (qId, &message, (size_t)nBytes, timeout) == OK)
			return OK;
		else
			return ERROR;
	}

	int recvMsg(ULONG qName, char *msg, ULONG maxNBytes, int timeout) {
		struct {
			long  mtype;
			char mtext[MAX_MSG_LEN];
		} message;

		if((rxBytes = msgrcv(qId, &message, (size_t)MAX_MSG_LEN, 1, timeout)) != -1) {
			bcopy(message.mtext, msg, rxBytes);
			return OK;
		}
		else
			return ERROR;
	}
};
