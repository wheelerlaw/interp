// Author: Wheeler Law
// Test program for symTbl.c

#define _GNU_SOURCE
#define BUFFER_SIZE 512

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "symTbl.h"
#include <string.h>

int readInput(char *message, char *buff);
Symbol createNewSymbol(Type type, char* varName, char* value);

int main(int argc, char* argv[]){	
	char buff[BUFFER_SIZE];
	while(readInput("Enter the file to load the symbol table from: ",buff)!=0){};
	
	// Test the build table and the dumpTable.
	buildTable(buff);
	dumpTable();
	
	while(readInput("Enter the character to look for: ",buff)!=0){};
	
	// Test the lookupTable.
	Symbol *symbol=lookupTable(buff);
	if(symbol!=NULL){
		if(symbol->value.type==TYPE_INT){
			printf("Type: int, Name: %s, Value: %d\n",symbol->varName,symbol->value.value.iVal);
		}else if(symbol->value.type==TYPE_DOUBLE){
			printf("Type: double, Name: %s, Value: %f\n",symbol->varName,symbol->value.value.dVal);
		}
	}else{
		printf("Symbol with name \"%s\" does not exist.\n",buff);
	}
	
	
	// Get input for next part
	while(readInput("Enter valid type: ",buff)!=0){};
	char *typeToken=(char *)malloc(strlen(buff)+1);
	strcpy(typeToken,buff);
	while(readInput("Enter valid name: ",buff)!=0){};
	char *nameToken=(char *)malloc(strlen(buff)+1);
	strcpy(nameToken,buff);
	while(readInput("Enter valid value: ",buff)!=0){};
	char *valueToken=(char *)malloc(strlen(buff)+1);
	strcpy(valueToken,buff);
	
	Symbol nS;
	if(strcmp(typeToken,"int")==0){
		nS=createNewSymbol(TYPE_INT, nameToken, valueToken);
	}else if(strcmp(typeToken,"double")==0){
		nS=createNewSymbol(TYPE_DOUBLE, nameToken, valueToken);
	}
	
	if(addTable(nS)==0){
		printf("Error adding to table!\n");
	}else{
		printf("Table successfully added to table.\n");
		dumpTable();
	}
	
	return 0;
	
}

/** Quick program to take input from the terminal. 
 ** @param *buff The preallocated character buffer to be used to read in.
 ** @return 1 if fail, 0 if success.
 **/
int readInput(char message[], char buff[]){
	int prompt=isatty(fileno(stdin));
	
	if(prompt){
		printf("%s", message);
	}
	if(fgets(buff,BUFFER_SIZE,stdin)==NULL){
		return 1;
	}
	
	int j=0;
	while(buff[j]!='\0' && buff[j]!='\n'){ //find the end of the buffer.
		++j;
	}
	
	if(buff[j]=='\n'){
		buff[j]='\0';
	}
	
	return 0;
}

Symbol createNewSymbol(Type type, char* varName, char* value){
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
