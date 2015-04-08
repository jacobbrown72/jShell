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
		
		checkalias = checkAlias();
		if(checkalias == SYSERR) 	{printf("Error: %s\n", errorMsg); continue;}
		if(checkalias == WARNING)	{printf("Warning: %s\n", warMsg);}
		
		checkcommand = checkCmd();
		if(checkcommand == SYSERR) 	{printf("Error: %s\n", errorMsg); continue;} 
		if(checkcommand == WARNING)	{printf("Warning: %s\n", warMsg);}
		
		ret = execute();
		if(ret == SYSERR)	{printf("Error: %s\n", errorMsg); continue;}
		if(ret == CLOSE)	{system("clear"); return 0;}
	}
}
