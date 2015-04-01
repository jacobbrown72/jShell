#include <stdio.h> 
#include "shell.h"


int main(){
	arg_counter = 0;
	cmd_counter = 0;
	/*
	for(i = 0; i < 5; i++){
		printf("jShell: ");
		yyparse();
	}
	*/
	printf("jshell: ");
	yyparse();
	Cmd my_cmd = cmd_table[0];
	printf("Command: %s\n", my_cmd.cmdname);
	int num_args = my_cmd.num_args;
	int i;
	for(i = 0; i < num_args; i++){
		printf("argument %d: %s\n", i, my_cmd.arguments[i]);
	}	
	return 0;
}
