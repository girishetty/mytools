/** 
 *  @file Stack.cpp
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

// INCLUDE FILES
#include "Stack.h"

//Global Stack Object for maintaining all the produced items
Stack stack;

/*****************************************************************************
*  CreateObserverThread
*  Function: Functions to Push Items on to Stack and Pop items from Stack
*  And also to delete Items
*******************************************************************************/

extern "C" 
{

void PushOntoStack( ProducedItem* aItem ) 
{
	stack.Push(aItem);
}

ProducedItem* PopFromStack() 
{
	return stack.Pop();
}

void FreeItem(ProducedItem* aItem) 
{
	delete aItem;
}

} //extern "C"

//  End of File
