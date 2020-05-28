/** 
 *  @file Stack.h
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
 
#ifndef STACK_H
#define STACK_H

/* INCLUDE FILES */
#include <string.h>
#include "CommanHeader.h"


/*
C++ Class that defines Stack
*/
class Stack {
enum 
	{
	EMaxProduction = 1000
	};

public:

	Stack() : iStatckTop(-1) 
	{
	}

	/**
	* Destructor
	* Delete all the items those are not yet consumed by the consumers
	*/
	~Stack()
	{
		ProducedItem* item = NULL; 
		do
			{
			item = Pop();
			delete item;
			} while(item != NULL);
	}

	/**
	* Push
	* New Item is created on Stack for every push operation
	* 
	* @param aItem - Item to be Pushed
	*
	* @return Nothing
	*/
	void Push( ProducedItem* aItem ) 
	{
		//If there is enough space to put item onto stack
		if(iStatckTop < EMaxProduction-1)
			{
			ProducedItem* item = new ProducedItem;
			memcpy(item, aItem, sizeof(ProducedItem));
			iItems[++iStatckTop] = item;
			}
		//else Ignore it!
	}

	/**
	* Pop
	* Item is sent to the Consumer who is doing the Pop. 
	* Ownership will also ne given.
	* So, this item has to be deleted by the Consumer.
	* 
	* @param  Nothing 
	*
	* @return - Item Poped
	*/
	ProducedItem* Pop() 
	{
		if(iStatckTop != -1) 
		{
			ProducedItem* item = iItems[iStatckTop];
			iItems[iStatckTop--] = NULL;
			return item;
		}
		return NULL;
	}

private:
	ProducedItem*	iItems[EMaxProduction];
	int				iStatckTop;
};

#endif /*STACK_H*/

/*  End of File */
