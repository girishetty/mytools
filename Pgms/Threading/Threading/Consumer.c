/** 
 *  @file Consumer.c
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


/* INCLUDE FILES */
#include <sys/types.h>
//#include <sys/ipc.h>
//#include <sys/msg.h>
#include <stdlib.h>

#include "CommanHeader.h"

/*****************************************************************************
*  ConsumerThreadEntryPoint
*  Function: Consumer Thread that consumes as amny as needed items when 
*  Producer has enough items produced
*  It also informs the Observer thread about item consumed
*******************************************************************************/

void* ConsumerThreadEntryPoint( void* aParam ) 
{
	ThreadParam* args = (ThreadParam*) aParam;
	int yetToConsume = args->noOfItems;
	int clientId = args->noOfConsumers;
	ProducedItem* consumeItem;
	int ret = 0;
	key_t msgQFd = -1;

	/* Construct the message to be send thru msg q */
	struct msgbuf* sendMsg = (struct msgbuf*)malloc(KMAXSENDMSG);
	sendMsg->mtype = 1;
	
	/* Get the Handler to Observer Msg Q */
	msgQFd = msgget(KMSGQKEY, IPC_CREAT);

	while (yetToConsume != 0) 
	{
		/* Acquire the Lock before checking for items available from Producer */
		sem_wait(&args->itemLock);
		/* Pop item from Producved Item-Stack */
		consumeItem = PopFromStack();
		if (consumeItem == NULL) 
		{
			/* No items in the Stack */
			sem_post(&args->itemLock);
			continue;
		}
		yetToConsume--;
		/* Inform the Observer about Item Consumption */
		sprintf(&sendMsg->mtext, " Consumer#%d Consuming Produced %s\0", clientId, consumeItem->itemName);
		ret = msgsnd(msgQFd, sendMsg, strlen(sendMsg->mtext)+4, 0);

		/* Release the Lock once after getting an item */
		sem_post(&args->itemLock);
		FreeItem(consumeItem);
	}

	free( sendMsg );
	return (int*)0;
}


/*  End of File */
