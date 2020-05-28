#include "msgq.h"

class Process {
	ULONG			processID;
	LogicalClock	oLogClk;
	pthread_t		rcvThread;
	volatile bool	recvFlag;
	MsgQueue		oMsgQ;
	BroadCastMsgQ	oBCMsgQ;
	ReqMsg			oReqMagQ[MAX_REQ];
	int				reqCount;

public:
	Process (int pID) : recvFlag (true), processID(pID), reqCount(0) {
		oMsgQ.createQueue (processID);
		oBCMsgQ.createQueue ();
		oLogClk.startClock();
		startRcvThread ();
	}

	~Process () {
		recvFlag = false;
		sleep(1000);
		oMsgQ.deleteQueue();
		oBCMsgQ.deleteQueue();
	}

	void startRcvThread () {
		if(pthread_create(&timerThread, NULL, (void *) Process::rcvMessages, NULL) != 0) {
			cout<<"Failed to create the thread"<<endl;
		}
	}

	static void rcvMessages () {
		Msgs msg;

		while(recvFlag) {
			if(oMsgQ.recvMsg(msg.sMsg, 100, 1)==ERROR)
				break;
			else {
				switch(msg.sMsg[0]){
				case kREQMSG_ID:
					processRequestMsg (oMsgQ.oReqMsg);
					break;
				case kANSMSG_ID:
					processAnsMsg (oMsgQ.oAnsMsg);
					break;
				case kRELEASEMSG_ID:
					processReleaseMsg (MsgQ.oReleaseMsg);
					break;
				}
			}
		}
	}

	void processRequestMsg (ReqMsg reqMsg){
		// Add the Request of that Process to Q
		if(reqCount < MAX_REQ)
			oReqMagQ[reqCount++] = reqMsg;
	}

	void processAnsMsg (AnsMsg ansMsg){
		// Now use the resource and release
		sendReleaseMsg ();
	}

	void processReleaseMsg (ReleaseMsg relMsg){
		// Delete the Request of that Process
		for(int i=0;i<reqCount;i++) {
			if(relMsg.ownId == oReqMagQ[i].ownId){
				for(int j=i;j<reqCount-1;j++)
					oReqMagQ[j] = oReqMagQ[j+1];
				reqCount--;
			}
		}
	}

	int sendRequestMsg () {
		Msgs msg;
		msg.oReqMsg.msgId = kREQMSG_ID;
		msg.oReqMsg.ownId = processID;
		msg.oReqMsg.time = oLogClk.getTime();

		return broacastMessage (msg);
	}

	int sendAnswerMsg () {
		Msgs msg;
		msg.oAnsMsg.msgId = kANSMSG_ID;
		msg.oAnsMsg.ownId = processID;
		msg.oAnsMsg.time = oLogClk.getTime();

		return broacastMessage (msg);
	}

	int sendReleaseMsg () {
		Msgs msg;
		msg.oReleaseMsg.msgId = kRELEASEMSG_ID;
		msg.oReleaseMsg.ownId = processID;
		msg.oReleaseMsg.time = oLogClk.getTime();

		return broacastMessage (msg);
	}

	int broacastMessage (Msgs msg) {
		return (oBCMsgQ.sendMsg((char *)&msg.sMsg, sizeof(Msgs), 1));
		}
	}
}
