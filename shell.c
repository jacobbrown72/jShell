#include <stdio.h> 
#include "shell.h"
#include "shellfunctions.h"
#include "shellCmds.h"

int main(){
	int i;
	while(1){
		resetShell();
		printf("jShell: ");
		yyparse();
		if(checkCmd() == OK){
			for(i = 0; i < cmd_counter; i++) execute(&cmd_table[i]);
		}
		else{
			printf("%s\n", errorMsg);
		}
	}
	return 0;
}
