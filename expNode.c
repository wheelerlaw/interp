// Wheeler Law
// Implementation of expNode.h

#include "expNode.h"
#include "symTbl.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/** Construct an expression node dynamically on the heap.
 ** Assume the following order when determining which
 ** type to create:
 **	1. the operators
 **  	2. literal
 **	3. integer literal
 **	4. symbol
 ** @param token The next C string token in the expression
 ** @param left Pointer to left node (NULL if none)
 ** @param right Pointer to right node (NULL if none)
 ** @return The new node
 **/
ExpNode* makeExpNode(char token[], ExpNode* left, ExpNode* right){
	ExpNode *newNode=(ExpNode *)malloc(sizeof(ExpNode));
	newNode->left=left;
	newNode->right=right;
	
	int isInteger=1;
	for(int i=0;i<strlen(token);i++){
		if(isdigit(token[i])==0){
			isInteger=0;
		}
	}
	
	if(!strcmp(token,ADD_OP_STR)){
		newNode->type=ADD_OP;
	}else if(!strcmp(token,SUB_OP_STR)){
		newNode->type=SUB_OP;
		
	}else if(!strcmp(token,MUL_OP_STR)){
		newNode->type=MUL_OP;
		
	}else if(!strcmp(token,DIV_OP_STR)){
		newNode->type=DIV_OP;
		
	}else if(!strcmp(token,MOD_OP_STR)){
		newNode->type=MOD_OP;
		
	}else if(!strcmp(token,ASSIGN_OP_STR)){
		newNode->type=ASSIGN_OP;
		
	}else if(strchr(token, (int)".")!=0){
		newNode->type=DOUBLE;
		newNode->value.value.dVal=atof(token);
		
	}else if(isInteger){
		newNode->type=INTEGER;
		newNode->value.value.iVal=atoi(token);
					
	}else{
		newNode->type=SYMBOL;
		strcpy(newNode->symbol,token);
	}
	
	return newNode;
}
