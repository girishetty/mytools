/** 
 *  @file Observer.cpp
 *  Part of     : Open C / ProducerConsumer
 *
 *  Description : ProducerConsumer Problem solution
 *  Version	   : 0.1
 *
 *  Copyright (c) 2007 Nokia Corporation.
 *  This material, including documentation and any related
 *  computer programs, is protected by copyright controlled by
 *  Nokia Corporation.
 */

#if 0
// INCLUDE FILES
#include <e32std.h>
#include <f32file.h>

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <pthread.h>

#include "CommanHeader.h"

_LIT(KObserverLogFile, "C:\\ObserverLogFile.txt");

/*****************************************************************************
*  ObserverThreadL
*  Function: Observer Thread that does the logging of all the events sent by 
*  Producer and Consumers
*******************************************************************************/

TInt ObserverThreadL( TInt aNoOfMsg ) 
	{
	TInt retVal = KErrNone;

	//Connect to Symbian File Server and open a file for logging
	RFs fileSession;
	User::LeaveIfError(fileSession.Connect());
	RFile logFile;
	retVal = logFile.Open(fileSession, KObserverLogFile, EFileWrite);
	if(retVal)
		{
		retVal = logFile.Create(fileSession, KObserverLogFile, EFileWrite);
		}
	User::LeaveIfError(retVal);

	//Create the MRT STDLIBS Message Q
    key_t msgQFd = msgget(KMSGQKEY, IPC_CREAT);
    if (msgQFd == -1)
    	{
    	logFile.Write(_L8("Msg Q Creation Failed\r"));
    	return -1;
    	}
   logFile.Write(_L8("Observer is Up and Running\r"));

	//Construct the message to be send thru msg q
	struct msgbuf* recvMsg = (struct msgbuf*)malloc(KMAXSENDMSG);
	TBuf8<KMAXSENDMSG> logData;
	
	
	for(int msgCount = 0; msgCount<aNoOfMsg; msgCount++ )
		{
		retVal = msgrcv(msgQFd, recvMsg, KMAXSENDMSG, 0, 0);
		if(retVal > 0 )
			{
			logData.Copy((const TUint8 *)recvMsg->mtext, retVal-4);
			logFile.Write(logData);
			//Also flush the info on to console
			recvMsg->mtext[retVal-4] = '\0';
			printf("Observer: %s\n", recvMsg->mtext);
			}
		}
	
	//Close the Message Q
	retVal = msgctl(msgQFd, IPC_RMID, NULL);
	free(recvMsg);
	
	logFile.Close();
	fileSession.Close();		
	return retVal;
	}

/*****************************************************************************
*  ObserverThreadEntryPoint
*  Function: Observer Thread Entry Point
*  As its Symbian Thread, it has to create a cleanup stack and should have
*  TOP level TRAP
*******************************************************************************/

TInt ObserverThreadEntryPoint( TAny* aParam ) 
	{
	TInt retVal = KErrNone;
	
	// Create a Cleanup Stack for this Thread
	CTrapCleanup* cleanupStack = CTrapCleanup::New();
	if(cleanupStack)
		{
		//Have a top level TRAP
		TRAP( retVal, retVal = ObserverThreadL( (int)aParam ));
		delete cleanupStack;
		}
	else
		{
		retVal = KErrNoMemory;
		}
	return retVal;
	}

extern "C" {

/*****************************************************************************
*  CreateObserverThread
*  Function: Function that creats Observing Thread (Symbian Thread)
*******************************************************************************/

void CreateObserverThread( int aNoOfMsg )
	{
	RThread thread;
	TInt stackSize = 0x8000; //Set the stack size for this thread as 8K
	thread.Create(_L("ObserverThread"), ObserverThreadEntryPoint, stackSize, NULL, (TAny*)aNoOfMsg);
	TRequestStatus stat;
	thread.Logon(stat);

	//Start executing the thread.
	thread.Resume();

	//Wait for the thread to Terminate.
	User::WaitForRequest(stat);
	}

} //extern "C"

//  End of File
#endif
