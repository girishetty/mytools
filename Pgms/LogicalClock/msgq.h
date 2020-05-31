#ifndef MSG_H
#define MSG_H

#include "types.h"

# define	kREQMSG_ID		0x20;
# define	kANSMSG_ID		0x21;
# define	kRELEASEMSG_ID	0x22;

# define	MAX_REQ			10


struct ReqMsg {
	BYTE msgId;
	int  ownId;
	long time;
};

struct AnsMsg {
	BYTE msgId;
	int ownId;
	long time;
};

struct ReleaseMsg {
	BYTE msgId;
	int ownId;
	long time;
};

union Msgs {
	ReqMsg		oReqMsg;
	AnsMsg		oAnsMsg;
	ReleaseMsg	oReleaseMsg;
	BYTE		sMas[20];
};

#endif //MSG_H
