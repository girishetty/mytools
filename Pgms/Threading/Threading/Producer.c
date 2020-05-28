/** 
 *  @file Producer.c
 *  Part of     : Open C / ProducerConsumer
 *
 *  Description : ProducerConsumer Problem solution
 *  Version:   0.1
 *
 *  Copyright (c) 2007 Nokia Corporation.
 *  This material, including documentation and any related
 *  computer programs, is protected by copyright controlled by
 *  Nokia Corporation.
 */


/* INCLUDE FILES */
#include <sys/types.h>
//#include <sys/ipc.h>
//#include <sys/msg.h>
#include <stdlib.h>
#include <stdio.h>

#include "CommanHeader.h"

#define MIN(X, Y) (X < Y) ? X : Y;

/*****************************************************************************
*  ProducerThreadEntryPoint
*  Function: Producer Thread that produces M number of items at a time 
*  where M is number of Consumers.
*  This thread will produce N number of items where N is total number of items 
*  needed by all the Consumers.
*  It also informs the Observer thread about item production
*******************************************************************************/

void* ProducerThreadEntryPoint( void* aParam ) 
{
	ThreadParam* args = (ThreadParam*) aParam;
	int yetToProduce = args->noOfItems;
	int produceNow = 0;
	int noOfConsumers = args->noOfConsumers;
	ProducedItem item;
	key_t msgQFd = -1;
	int ret = 0;
	/* Construct the message to be send thru msg q */
	struct msgbuf* sendMsg = (struct msgbuf*)malloc(KMAXSENDMSG);
	sendMsg->mtype = 1;
	
	/* Get the Handler to Observer Msg Q */
	msgQFd = msgget(KMSGQKEY, IPC_CREAT);

	item.itemNum = 1;

	while (yetToProduce != 0) 
	{
		produceNow = MIN(yetToProduce, noOfConsumers);
		yetToProduce -= produceNow;

		/* Acquire the Lock so that Producer can Produce "produceNow" items once. */
		sem_wait(&args->itemLock);
		for(;produceNow != 0; produceNow--) 
		{
			sprintf(item.itemName, "Item-Number: #%d", item.itemNum);
			/* Push this item on to Stack so that Consumetrs can take it.*/
			PushOntoStack(&item);
			item.itemNum++;
			/* Inform the Observer about Item Production */
			sprintf(sendMsg->mtext, " Producer Produced %s\0", item.itemName);
			ret = msgsnd(msgQFd, sendMsg, strlen(sendMsg->mtext)+4, 0);
		}
		/* Release the Lock so that Consumers can consume produced items. */
		sem_post(&args->itemLock);
	}

	free( sendMsg );
	return (int*)0;
}

/*  End of File */
