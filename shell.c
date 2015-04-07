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
		switch(checkCmd()){
			case OK: /* execute builtin command */
				for(i = 0; i < cmd_counter; i++){
					ret = execute(&cmd_table[i]);
				}
				break;
			case OTHERCMD: /* execute non-builtin command */
				executeOther();
				break;
			default:
				if(strcmp(errorMsg,"") == 0){
					printf("An unknown error occured...\n");
				}else{
					printf("%s\n", errorMsg);
				}
		}
		if(ret == CLOSE) return 0;
	}
	return 0;
}
