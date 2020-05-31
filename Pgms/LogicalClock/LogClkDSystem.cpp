# define SET_TIME   0x10

struct TimeMsg {
	int msgId;
	long time;
};

class LogClkDSystem {
	ULONG			processID;
	LogicalClock	oLogClk;
	pthread_t		rcvThread;
	volatile bool	recvFlag;
	MsgQueue		oMsgQ;

public:
	LogClkDSystem (int pID) : recvFlag (true), processID(pID) {
		oMsgQ.createQueue (processID);
		oLogClk.startClock();
		startRcvThread ();
	}

	~LogClkDSystem () {
		recvFlag = false;
		sleep(1000);
		oMsgQ.deleteQueue();
	}

	void startRcvThread () {
		if(pthread_create(&timerThread, NULL, (void *) LogClkDSystem::rcvMessages, NULL) != 0) {
			cout<<"Failed to create the thread"<<endl;
		}
	}

	static void rcvMessages () {
		char msg [100];
		TimeMsg  oMsg;

		while(recvFlag) {
			if(oMsgQ.recvMsg(msg, 100, 1)==ERROR)
				break;
			else {
				memcpy(msg, oMsg, sizeof(oMsg));
				if(oMsg.msgId==SET_TIME)
					if(oLogClk.getTime() < oMsg.time)
						oLogClk.setTime(oMsg.time)
			}
		}
	}

	int sendMessage () {
		TimeMsg  oMsg;
		oMsg.msgId = SET_TIME;
		oMsg.time = oLogClk.getTime();
		return (oMsgQ.sendMsg((char *)&oMsg, sizeof(oMsg), 1));
		}
	}
}
