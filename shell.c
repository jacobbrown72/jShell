#include <stdio.h> 
#include "shell.h"


int main(){
	int i;
	for(i = 0; i < 5; i++){
		printf("jShell: ");
		yyparse();
	}
	printf("\n\noutputing inputed commands\n");
	for(i = 0; i < cmd_counter; i++){
		Cmd my_cmd = cmd_table[i];
		printf("Command: %s\nCommand number: %d\n\n", my_cmd.cmdname, my_cmd.bi_type);
	}
	return 0;
}
