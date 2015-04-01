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
	if(inFile_red){
		printf("Input file: %s\n", inFile);
	}
	if(outFile_red){
		printf("Output file: %s\n", outFile);
	}
	if(errFile_red){
		printf("Error file: %s\n", errFile);
	}
}

void resetShell(){
	/*reset arg and cmd counter*/
	arg_counter = 0;
	cmd_counter = 0;
	
	inFile_red = 0;
	outFile_red = 0;
	errFile_red = 0;
	append = 0;
	
	amp = 0;
}

void checkCmd(){

}

int main(){
	
	while(1){
		resetShell();
		printf("jShell: ");
		yyparse();
		print_commands();
	}
	return 0;
}
