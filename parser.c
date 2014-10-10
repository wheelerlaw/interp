// Author: Wheeler Law
// Implementation of parser.h


#include "parser.h"
#include "stack.h"
#include "expNode.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

Symbol createSymbol(Type type, char* varName, char* value);

ParserError pE=NONE;

/** The main parse routine that handles the entire parsing
 ** process, using the rest of the routines defined here.
 ** @param exp The expression as a string
 **/
void parse(char exp[]){
	ExpNode *rootNode=parseTree(exp);
	Value result=evalTree(rootNode);
	
	if(pE==NONE){
		infixTree(rootNode);
		printf(" = ");
		if(result.type==TYPE_DOUBLE){
			printf("%f\n",result.value.dVal);
		}else if(result.type==TYPE_INT){
			printf("%d\n",result.value.iVal);
		}
	}
	
	cleanupTree(rootNode);
	
	
}

/** Constructs the expression tree from the expression.  It
 ** must use the stack to order the tokens.  It must also
 ** deallocate the memory associated with the stack in all cases.
 ** If a symbol is encountered, it should be stored in the node
 ** without checking if it is in the symbol table - evaluation will
 ** resolve that issue.
 ** @param expr the postfix expression as a C string
 ** @return the root of the expression tree
 ** @exception There are 2 error conditions that you must deal
 ** 	with.  In each case, the memory associated with the
 **	tree must be cleaned up before returning.  Neither 
 **	stops execution of the program. 
 **
 **	1. If there are too few tokens, set the parser error
 **	to TOO_FEW_TOKENS and display the message to standard error:
 **
 **	Invalid expression, not enough tokens
 **
 **	2. If there are many tokens (stack is not empty after building),
 **	set the parser error to TOO_MANY_TOKENS and display the message
 **	to standard error:
 **
 ** Invalid expression, too many tokens
 **/
ExpNode* parseTree(char expr[]){

	// Strtok is undefined for using string literals (char[]) for the first
	// argument so we are going to duplicate it here dynamically.
	char *strCopy=malloc(strlen(expr)+1);
	strcpy(strCopy,expr);
	
	// Create the stack.
	StackNode *stack=NULL;

	// Pointer used for the token.
	char *token;
	
	// Pointer used to hold address of current node.
	ExpNode *newNode;
	
	// A pointer to hold the string of the address of the newNode to put in the stack.
	char *tempStr=malloc(11); 
	
	int runOnce=0;
	while(runOnce=runOnce?((token=strtok(NULL," "))!=NULL):((token=strtok(strCopy," "))!=NULL)){
		newNode=makeExpNode(token,NULL,NULL); // Make the new node.
		
		if(newNode->type==INTEGER || newNode->type==DOUBLE || newNode->type==SYMBOL){
			// Cast pointer to integer, then convert integer to string, add string to stack.
			sprintf(tempStr,"%d",(int)newNode);
			push(&stack,tempStr);
			
		}else if(newNode->type==ADD_OP || newNode->type==SUB_OP || newNode->type==MUL_OP || newNode->type==DIV_OP || newNode->type==MOD_OP || newNode->type==ASSIGN_OP){
			
			// If the stack is empty at this point, there was a shortage of tokens.
			if(emptyStack(stack)){
				fprintf(stderr,"Invalid expression, not enough tokens\n");
				pE=TOO_FEW_TOKENS;
				exit(EXIT_FAILURE);
			}
			
			// Convert the string into an integer, cast it to an ExpNode pointer.
			newNode->left=(ExpNode *)atoi(top(stack));
			pop(&stack);
			
			if(emptyStack(stack)){
				fprintf(stderr,"Invalid expression, not enough tokens\n");
				pE=TOO_FEW_TOKENS;
				exit(EXIT_FAILURE);
			}
			newNode->right=(ExpNode *)atoi(top(stack));
			pop(&stack);
			
			// Take the new node with new children, and convert it again and push onto stack.
			sprintf(tempStr,"%d",(int)newNode);
			push(&stack,tempStr);
		}
	}
	
	// Free dynamic memory associated with this function.
	free(strCopy),free(tempStr);
	
	// The last node on the stack is the root of the tree.
	newNode=(ExpNode *)atoi(top(stack));
	pop(&stack);
	
	// If there are excess nodes, there were too many tokens.
	if(!emptyStack(stack)){
		fprintf(stderr,"Invalid expression, too many tokens");
		pE=TOO_MANY_TOKENS;
		exit(EXIT_FAILURE);
	}
	
	return newNode;
}

/** Evaluates the tree and returns the result.
 ** @param node The node in the tree
 ** @precondition:  This routine should not be called if there
 ** 	is a parser error.
 ** @return the evaluated value.  Note:  A symbol evaluates
 **	to its stored value.  
 **
 **	The evaluator needs to be able to deal with the multiple
 **	types our interpreter supports (TYPE_INT and TYPE_DOUBLE).
 **
 **	1. If a symbol is not in the table, and the table isn't full
 **	It should be added to the table taking on the type of the
 **	rhs (right hand side) of the assignment.  Note that
 **	a symbol will evaluate to its stored value (and type).
 **
 **	2. When performing the math operations (except for modulus),
 **	the following rule applies.  If both lhs and rhs are int's,
 **	the result is an int, otherwise the result is a double.
 **
 ** @exceptions There are 6 error conditions that can occur.  If
 **	either occurs, set the correct parser error and display
 **	an error message and return a Value of TYPE_UNKNOWN.   The main program
 **	should check the parser error state before using the
 **	return result.
 **
 ** 1. If a symbol is referenced on the left hand side during an
 ** 	assignment, it should be added to the symbol table with
 ** 	the value being the evaluation of the right hand side.  If
 ** 	there is no more room in the symbol table, it should set
 ** 	a SYMBOL_TABLE_FULL parser error and display the following
 ** 	message to standard error
 **
 ** 	Symbol table full, cannot create new symbol
 **
 ** 2. An assignment is made to a left hand side expression
 **	node that is not a symbol node.  It should set an 
 **	INVALID_ASSIGNMENT parser error and display the following
 **	message to standard error:
 **
 **	Invalid assignment
 **
 ** 3. An assignment is made to a symbol with a value whose
 **	type does not match the symbol's type (e.g. assigning
 **	a double value to an int symbol).  It should set
 **	an INVALID_ASSIGNMENT parser error, and display the following
 ** 	message to standard error:
 **
 **	Assignment type mismatch
 **
 ** 4. If a math operation is being performed on a symbol that
 **	that does not exist, you should set an UKNOWN_SYMBOL
 ** 	parser error and display the following message to standard error
 **     (where <symbol-name> is the name of the symbol):
 **
 **	Unknown symbol: <symbol-name>
 **
 ** 5. If modulus (%) is performed, both left and right hand side nodes
 **	must be TYPE_INT.  If this happens, do not perform the operation,
 **	instead set the parser error to INVALID_MODULUS and display
 ** 	the following error to standard error:
 **
 **	Modulus requires both types to be int
 **
 ** 6. If division by zero (or modulus) is being performed, do not do it.  
 **     Instead set the parser error to DIVISION_BY_ZERO and display the
 **	following error to standard error:
 **
 **	Division by zero
 **/
 Value evalTree(ExpNode* node){
 
	// Create the empty unkown Value in case of an error.
	Value unknown;
	unknown.type=TYPE_UNKNOWN;
 
	if(node==NULL){
		return unknown;
	}
	
	// Integer
	if(node->type==INTEGER){
		return node->value;
	}
	
	// Double
	if(node->type==DOUBLE){
		return node->value;
	}
	
	// Symbol
	if(node->type==SYMBOL){
		Symbol *symbol;
		if(symbol=lookupTable(node->symbol)){
			return symbol->value;
			
		// If not currently assigning to a symbol, and symbol not in the table,
		// then it must be invalid.
		}else{
			pE=UNKNOWN_SYMBOL;
			fprintf(stderr,"Unknown symbol: %s\n",node->symbol);
			return unknown;
		}
	}
	
	// Assignment
	if(node->type==ASSIGN_OP){
	
		// Check if the left side is a symbol
		if(node->left->type!=SYMBOL){
			pE=INVALID_ASSIGNMENT;
			fprintf(stderr,"Invalid assignment\n");
			return unknown; 
		}
		
		// Get the right value and make sure the types match the left value:
		Value rightValue=evalTree(node->right);
		if(rightValue.type=TYPE_UNKNOWN){ //Return the unknown value if there was a problem with the expression
			return rightValue;
		}
		// Using evalTree instead of reaching directly into the node allows it to evaluate
		// the expression on the right hand side before assigning it to the symbol.
		// But we don't want to do that on the left side because otherwise, since the symbol doesn't
		// exist yet in the symbol table, would return an unknown value.
		
		if(node->left->value.type!=rightValue.type){
			pE=INVALID_ASSIGNMENT;
			fprintf(stderr,"Assignment type mismatch\n");
			return unknown; 
		}
		
		// All clear to make a new symbol:
		struct Symbol nS;
		strcpy(nS.varName,node->left->symbol);
		nS.value.type=rightValue.type;
		if(rightValue.type==TYPE_INT){
			nS.value.value.iVal=node->left->value.value.iVal;
		}else if(rightValue.type==TYPE_DOUBLE){
			nS.value.value.dVal=node->left->value.value.dVal;
		}
		
		if(!addTable(nS)){
			pE=SYMBOL_TABLE_FULL;
			fprintf(stderr,"Symbol table full, cannot create new symbol\n");
			return unknown;
		}
		
		return rightValue;
	}
	
	
	// Can get both values since were not doing assignment.
	Value leftValue=evalTree(node->left);
	Value rightValue=evalTree(node->right);
	
	//Divide by zero, invalid symbol, etc. Just need to keep passing it up.
	if(leftValue.type==TYPE_UNKNOWN){
		return leftValue;
	}else if(rightValue.type==TYPE_UNKNOWN){
		return rightValue;
	}
	
	// Addition
	if(node->type==ADD_OP){
		
		Value result;
		if(leftValue.type==TYPE_DOUBLE && rightValue.type==TYPE_DOUBLE){
			result.type=TYPE_DOUBLE;
			result.value.dVal=leftValue.value.dVal+rightValue.value.dVal;
		}else if(leftValue.type==TYPE_DOUBLE && rightValue.type==TYPE_INT){
			result.type=TYPE_DOUBLE;
			result.value.dVal=leftValue.value.dVal+rightValue.value.iVal;
		}else if(leftValue.type==TYPE_INT && rightValue.type==TYPE_DOUBLE){
			result.type=TYPE_DOUBLE;
			result.value.dVal=leftValue.value.iVal+rightValue.value.dVal;
		}else{
			result.type=TYPE_INT;
			result.value.iVal=leftValue.value.iVal+rightValue.value.iVal;
		}
		return(result);
	}
	
	// Subtraction
	if(node->type==SUB_OP){
		
		Value result;
		if(leftValue.type==TYPE_DOUBLE && rightValue.type==TYPE_DOUBLE){
			result.type=TYPE_DOUBLE;
			result.value.dVal=leftValue.value.dVal-rightValue.value.dVal;
		}else if(leftValue.type==TYPE_DOUBLE && rightValue.type==TYPE_INT){
			result.type=TYPE_DOUBLE;
			result.value.dVal=leftValue.value.dVal-rightValue.value.iVal;
		}else if(leftValue.type==TYPE_INT && rightValue.type==TYPE_DOUBLE){
			result.type=TYPE_DOUBLE;
			result.value.dVal=leftValue.value.iVal-rightValue.value.dVal;
		}else{
			result.type=TYPE_INT;
			result.value.iVal=leftValue.value.iVal-rightValue.value.iVal;
		}
		return(result);
	}
	
	// Multiplication
	if(node->type==MUL_OP){
		
		Value result;
		if(leftValue.type==TYPE_DOUBLE && rightValue.type==TYPE_DOUBLE){
			result.type=TYPE_DOUBLE;
			result.value.dVal=leftValue.value.dVal*rightValue.value.dVal;
		}else if(leftValue.type==TYPE_DOUBLE && rightValue.type==TYPE_INT){
			result.type=TYPE_DOUBLE;
			result.value.dVal=leftValue.value.dVal*rightValue.value.iVal;
		}else if(leftValue.type==TYPE_INT && rightValue.type==TYPE_DOUBLE){
			result.type=TYPE_DOUBLE;
			result.value.dVal=leftValue.value.iVal*rightValue.value.dVal;
		}else{
			result.type=TYPE_INT;
			result.value.iVal=leftValue.value.iVal*rightValue.value.iVal;
		}
		return(result);
	}
	
	// Division
	if(node->type==DIV_OP){
		
		Value result;
		if(rightValue.value.iVal==0 || rightValue.value.dVal==0){
			pE=DIVISION_BY_ZERO;
			fprintf(stderr,"Division by zero\n");
			return unknown;
		}
		if(leftValue.type==TYPE_DOUBLE && rightValue.type==TYPE_DOUBLE){
			result.type=TYPE_DOUBLE;
			result.value.dVal=leftValue.value.dVal/rightValue.value.dVal;
		}else if(leftValue.type==TYPE_DOUBLE && rightValue.type==TYPE_INT){
			result.type=TYPE_DOUBLE;
			result.value.dVal=leftValue.value.dVal/rightValue.value.iVal;
		}else if(leftValue.type==TYPE_INT && rightValue.type==TYPE_DOUBLE){
			result.type=TYPE_DOUBLE;
			result.value.dVal=leftValue.value.iVal/rightValue.value.dVal;
		}else{
			result.type=TYPE_INT;
			result.value.iVal=leftValue.value.iVal/rightValue.value.iVal;
		}
		return(result);
	}
	
	// Modulus
	if(node->type==MOD_OP){
		
		Value result;
		if(leftValue.type==TYPE_DOUBLE || rightValue.type==TYPE_DOUBLE){
			pE=INVALID_MODULUS;
			fprintf(stderr,"Modulus requires both types to be int\n");
			return unknown;
			
		}else if(rightValue.value.iVal==0 || rightValue.value.dVal==0){
			pE=DIVISION_BY_ZERO;
			fprintf(stderr,"Division by zero\n");
			return unknown;
		}
		result.type=TYPE_INT;
		result.value.iVal=leftValue.value.iVal%rightValue.value.iVal;
		return(result);
	}
	
	return unknown;
}

/** Displays the infix expression for the tree, using
 ** parentheses to indicate the precedence, e.g.:
 **
 ** expression: 10 20 + 30 *
 ** infix string: ((10 + 20) * 30) 
 **
 ** @precondition:  This routine should not be called if there
 ** 	is a parser error.
 **/
void infixTree(ExpNode* node){
	
	if(node->type==INTEGER){
		printf("%d",node->value.value.iVal);
		return;
	}
	
	if(node->type==DOUBLE){
		printf("%f",node->value.value.dVal);
		return;
	}
	
	if(node->type==SYMBOL){
		printf("%s",node->symbol);
		return;
	}
	
	printf("(");
	infixTree(node->left);
	if(node->type==ADD_OP){
		printf(" + ");
	}else if(node->type==SUB_OP){
		printf(" - ");
	}else if(node->type==MUL_OP){
		printf(" * ");
	}else if(node->type==DIV_OP){
		printf(" / ");
	}else if(node->type==MOD_OP){
		printf(" %% ");
	}else if(node->type==ASSIGN_OP){
		printf(" = ");
	}
	infixTree(node->right);
	printf(")");
}

/** Cleans up all dynamic memory associated with the expression tree.
 ** @param node The current node in the tree
 **/
void cleanupTree(ExpNode* node){
	if(node==NULL){
		return;
	}
	cleanupTree(node->left);
	free(node->left);
	
	cleanupTree(node->right);
	free(node->right);
}	

/** Tells the main program whether there was an error with either
 ** the parsing or evaluation of the tree.
 ** @return parser error
 **/
ParserError getParserError(void){
	return pE;
}

