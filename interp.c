// Author: Wheeler Law
// Implementation of interp.h

#define _GNU_SOURCE

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "interp.h"
#include "symTbl.h"

/**
 ** Runs the entire program. Takes in command line arguments, calls parser
 ** function as many times as specified. Prints out symbol table. Etc.
 ** @param argc The number of arguments including the command itself.
 ** @param *argv[] An array of strings of the arguments.
 **/
int main(int argc, char* argv[]){
	int prompt=isatty(fileno(stdin));
	
	if(argc==2){
		buildTable(argv[1]);
	}else if(argc==1){
		buildTable(NULL);
	}else{
		fprintf(stderr,"Usage: interp [sym-table]\n");
		exit(EXIT_FAILURE);
	}
	
	dumpTable();
	
	
	char buff[MAX_LINE];
	
	printf("Enter postfix expressions (CRTL-D to exit): \n");
	
	while(1){
		
		printf("> ");
		
		if(fgets(buff,MAX_LINE,stdin)==NULL){
			return 1;
		}
		
		int j=0;
		while(buff[j]!='\0' && buff[j]!='\n'){
			j++;
		}
		
		if(buff[j]=='\n'){
			buff[j]='\0';
		}
		
		parse(buff);
		
		ParserError pE=getParserError();
		
	}
		

	return 0;
}
