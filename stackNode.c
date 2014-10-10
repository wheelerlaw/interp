/*
 * Wheeler Law
 * Implementation of stackNode.h. 
 * Started: 10/25/2013
 * Finished: 11/6/13
 */
 
#include "stackNode.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/**
 ** Dynamically allocates the stack node.
 ** @param data The data value.
 ** @param nedt The pointer to the next node in the stack (NULL if none).
 ** @return The new stack node.
 **/
 StackNode* makeStackNode(char* data, StackNode* next){
	StackNode *sN=NULL;
	sN=(StackNode *)malloc(sizeof(StackNode));
	
	sN->data=(char *)malloc(strlen(data)+1);
	strcpy(sN->data,data);
	
	sN->next=next;
	
	return sN;
}
	