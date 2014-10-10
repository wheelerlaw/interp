// Test program for the stack implementation. 
// Author: Wheeler Law

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stack.h"
#include "stackNode.h"

int main(){

	printf("A simle program to test the stack implementation.\nThe character buffer is 64 characters long.\n");
	char buff[64];
	
	StackNode *stack=NULL;
	
	while(1){
		printf("Enter value to be put on the stack: ");
		if(fgets(buff,64,stdin)==NULL){
			return 1;
		}
		
		int j=0;
		while(buff[j]!='\0' && buff[j]!='\n'){
			j++;
		}
		if(j==0){
			break;
		}
		if(buff[j]=='\n'){
			buff[j]='\0';
		}
		
		push(&stack,buff);
	}
	
	printf("Printing stack: \n");
	while(!emptyStack(stack)){
		char *data=NULL;
		data=top(stack);
		pop(&stack);
		printf("%s\n",data);
	}
	
	return 0;
}
		
		