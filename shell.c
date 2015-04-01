#include <stdio.h> 
#include "shell.h"

void print_commands(){
	int i;
	int j;
	for(i = 0; i < cmd_counter; i++){
		Cmd my_cmd = cmd_table[i];
		printf("Command: %s\n", my_cmd.cmdname);
		for(j = 0; j < my_cmd.num_args && my_cmd.num_args > 0; j++){
			printf("Argument %d: %s\n", j+1, my_cmd.arguments[j]);
		}
	}
}

int main(){
	arg_counter = 0;
	cmd_counter = 0;
	
	while(1){
		printf("jShell: ");
		yyparse();
		print_commands();
		cmd_counter = 0;
	}
	return 0;
}
