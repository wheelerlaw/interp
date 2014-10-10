// Implementation of symTbl.h
// Author: Wheeler Law

#define BUFFER_SIZE 1024
#define _GNU_SOURCE

#include "symTbl.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static Symbol createSymbol(Type type, char* varName, char* value);

// The symbol table. Statically and globally defined to be accessed in this file
// only and across all functions.
static Symbol sT[MAX_SYMBOLS];
static int numSymbols;


/** Constructs the table by reading the file.  The format is
 ** one symbol per line in the format:
 **
 **	variable-type variable-name	variable-value
 **	...
 **
 ** For this problem we guarantee the variable-names are all
 ** unique, and the format of the file is error free.
 ** You are allowed to create it statically or
 ** dynamically and store it locally.
 ** @param filename The name of the file containing the symbols
 ** @exception If the file can't be opened, an error message should
 ** be displayed to standard output and the program should exit with
 ** EXIT_FAILURE. If no file specified, an empty table should be built.
 **
 ** Error loading symbol table
 **/
void buildTable(char filename[]){
	if(filename==NULL){ //no filename, empty table.
		return;
	}
	FILE *file=fopen(filename,"r");
	
	if(file==NULL){
		fprintf(stderr,"Error loading symbol table\n");
		exit(EXIT_FAILURE);
	}
	
	int i=0;
	while(i<MAX_SYMBOLS && !feof(file)){
		
		char *typeToken=(char *)malloc(BUFFER_SIZE);
		char *nameToken=(char *)malloc(BUFFER_SIZE);
		char *valueToken=(char *)malloc(BUFFER_SIZE);
		
		if(fscanf(file, "%s %s %s", typeToken, nameToken, valueToken)!=3){
			break;
		}
		
			
		if(strcmp(typeToken,"int")==0){
			Symbol nS=createSymbol(TYPE_INT, nameToken, valueToken);
			sT[i]=nS;
		}else if(strcmp(typeToken,"double")==0){
			Symbol nS=createSymbol(TYPE_DOUBLE, nameToken, valueToken);
			sT[i]=nS;
		}
		
		i++;
	}
	numSymbols=i;
	
	fclose(file);
	
	return;
}

/** Displays the contents of the symbol table in the following format:
 **
 ** SYMBOL TABLE:
 **	Type: variable-type Name: variable-name, Value: variable-value
 **	...
 **
 ** Each symbol should be printed one per line, tab-indented.
 **/
void dumpTable(void){
	printf("SYMBOL TABLE:\n");
	int i=0;
	while(i<numSymbols){
		if(sT[i].value.type==TYPE_INT){
			printf("Type: int, Name: %s, Value: %d\n",sT[i].varName,sT[i].value.value.iVal);
		}else if(sT[i].value.type==TYPE_DOUBLE){
			printf("Type: double, Name: %s, Value: %f\n",sT[i].varName,sT[i].value.value.dVal);
		}
		i++;
	}
}

/** Returns the symbol associated with variable name
 ** @param symbol The name of the variable
 ** @return The Symbol, if it exists, or NULL otherwise
 **/	
Symbol* lookupTable(char variable[]){
	int i=0;
	while(i<numSymbols){
		if(strcmp(variable,sT[i].varName)==0){
			return &sT[i];
		}
		i++;
	}
	return NULL;
}

/** Attempt to add a new symbol to the table (if it is not
 ** already full).
 ** @param Symbol symbol the new symbol to add
 ** @return 0 if the symbol could not be added, anything else otherwise
 **/
int addTable(Symbol symbol){
	if(numSymbols<MAX_SYMBOLS){
		sT[numSymbols]=symbol;
		numSymbols++;
		return numSymbols;
	}else{
		return 0;
	}
}

/// Destroys the symbol table (if you built it dynamically).
void destroyTable(){}
	

/**
 ** Creates the symbol from the parameters.
 ** @param type The type of the symbol.
 ** @param varName The name of the symbol.
 ** @param value The value of the symbol.
 ** @return nS The completed symbol. 
 **/
 Symbol createSymbol(Type type, char* varName, char* value){
	struct Symbol nS;
	strcpy(nS.varName,varName);
	nS.value.type=type;
	if(type==TYPE_INT){
		nS.value.value.iVal=atoi(value);
	}else if(type==TYPE_DOUBLE){
		nS.value.value.dVal=atof(value);
	}
	
	return nS;
}
