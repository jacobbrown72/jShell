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
		checkAlias();
		if(checkCmd() == OK){
			for(i = 0; i < cmd_counter; i++){
				ret = execute(&cmd_table[i]);
			}
		}
		else{
			printf("%s\n", errorMsg);
		}
		if(ret == CLOSE) return 0;
	}
	return 0;
}
