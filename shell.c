#include <stdio.h>
#include "shell.h"
#include "shellfunctions.h"
#include "shellCmds.h"

int main(){
	int i;
	initShell();
	while(1){
		resetShell();
		printPrompt();
		yyparse();
		if(!checkAlias()) 	{printf("Error: %s\n", errorMsg); continue;}
		if(!checkCmd()) 	{printf("Error: %s\n", errorMsg); continue;} 
		ret = execute();
		if(ret == SYSERR)	{printf("Error: %s\n", errorMsg); continue;}
		if(ret == CLOSE)	return 0;
	}
}

/*
	checkAlias();
		if(checkCmd() == OK){
			ret = execute();
		}
		else{
			printf("%s\n", errorMsg);
		}
		if(ret == CLOSE) return 0;
	}
	return 0;
*/
