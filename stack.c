/*
 * Wheeler Law
 * Stack implementation of stack.h
 * 
 * Started: 10/23/13
 * Finished: 11/6/13
 */
 
#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

/** Add an element to the top of the stack (stack is changed).
 ** Dynamicaly allocates the space for the new node through makeStackNode();
 ** @param stack Points to the top of the stack.
 ** @param data The token (C string).
 **/
void push(StackNode** stack, char* data){
	if(*stack==NULL){
		StackNode *newNode=makeStackNode(data,NULL);
		*stack=newNode;
	}else{
		StackNode *newNode=makeStackNode(data,*stack);
		*stack=newNode;
	}
}
	
/** Return the top element from the stack (stack is unchanged).
 ** @param stack Points to the top of the stack.
 ** @return The top element on the stack. 
 ** @exception If the stack is empty, the program should exit with EXIT_FAILURE.
 **/
 char* top(StackNode* stack){
	if(stack==NULL){
		exit(EXIT_FAILURE);
	}
	return(stack->data);
}

/**
 ** Removes the top element from the stack (stack is changed).
 ** This routine should free the memory associated with the top node.
 ** @param stack Points to the top of the stack.
 ** @exception If the stack is empty, the program should exit with EXIT_FAILURE.
**/
void pop(StackNode** stack){
	if(*stack==NULL){
		exit(EXIT_FAILURE);
	}
	StackNode *tempNode=(*stack)->next;
	free(*stack);
	*stack=tempNode;
}
	
/** Tells whether the stack is empty or not (stack is unchanged)
 ** @param stack Points to the top of the stack
 ** @return 0 if not empty, any other value otherwise
 **/
int emptyStack(StackNode* stack){
	if(stack==NULL){
		return 1;
	}
	return 0;
}
