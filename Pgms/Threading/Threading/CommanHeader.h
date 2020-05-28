/** 
 *  @file CommandHeader.h
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


#ifndef COMMANHEADER_H
#define COMMANHEADER_H

/* INCLUDE FILES */
#include <semaphore.h>

#define MAX_CLIENTS 	10
#define KPortNum 		2222
#define KMSGQKEY 		1000
#define KMAXSENDMSG  	100

#define	IN_SET_LOOPBACK_ADDR(a) \
	{ (a)->sin_addr.s_addr  = 0x0100007F; (a)->sin_family = AF_INET; }


/* Structure used for sending details to the Producer and Consumers */
typedef struct 
{
	int   noOfItems;
	int   noOfConsumers;
	sem_t itemLock;
} ThreadParam;

/* Structure defining Items to be Produced */
typedef struct 
{
	int    itemNum;
	char   itemName[50];
} ProducedItem;



/**
 * Declaration of Producer Thread Entry Point 
 * @param aParam - argument to thread Entry function
 *
 * @return thread return value
 */
void* ProducerThreadEntryPoint( void* aParam );

/**
 * Declaration of Consumer Thread Entry Point 
 * @param aParam - argument to thread Entry function
 *
 * @return thread return value
 */
void* ConsumerThreadEntryPoint( void* aParam );

#ifdef __cplusplus
extern "C" 
{
#endif
/**
 * Declaration of Function that creats Observing Thread
 * @param aNoOfMsg - Number of messages
 *
 * @return Nothing
 */
void CreateObserverThread( int aNoOfMsg );

/**
 * Declaration of Function that Push Item onto Stack
 * @param aItem - Item to be pushed
 *
 * @return Nothing
 */
void PushOntoStack( ProducedItem* aItem );

/**
 * Declaration of Function that Pop Item from Stack
 * @return aItem - Item Poped
 */
ProducedItem* PopFromStack();

/**
 * Declaration of Function that frees up Item
 * @param aItem - Item to be Deleted
 * @return Nothing
 */
void FreeItem(ProducedItem* aItem);

#ifdef __cplusplus
}
#endif


#endif /*COMMANHEADER_H*/

/*  End of File */
