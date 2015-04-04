#include <stdio.h>
#include <string.h>
#include "shell.h"
#include "shellfunctions.h"
#include "shellCmds.h"

void printCommands(){
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

void initShell(){
	int i;
	int j;
	Cmd* cmd;
	Env* env;
	Alias* alias;
	
	/*initialize command table*/
	for(i = 0; i < MAXCMDS; i++){
		cmd = &cmd_table[i];
		strcpy(cmd->cmdname, "");
		cmd->bi_type = -1;
		for(j = 0; j < MAXARGS; j++){
			strcpy(cmd->arguments[j], "");
		}
		cmd->num_args = 0;
		cmd->infd = -1;
		cmd->outfd = -1;
	}
	
	/*initialize IO variables*/
	inFile_red = 0;
	outFile_red = 0;
	errFile_red = 0;
	append = 0;
	strcpy(inFile, "");
	strcpy(outFile, "");
	strcpy(errFile, "");
	amp = 0;
	
	/*initialize env table*/
	for(i = 0; i < MAXENV; i++){
		env = &env_table[i];
		strcpy(env->variable, "");
		strcpy(env->path, "");
		env->used = 0;
	}
	
	/*initialize alias table*/
	for(i = 0; i < MAXALI; i++){
		alias = &alias_table[i];
		strcpy(alias->name, "");
		strcpy(alias->value, "");
		alias->used = 0;
	}
}

void resetShell(){
	/*reset arg and cmd counter*/
	arg_counter = 0;
	cmd_counter = 0;
	bi = 0;
	
	inFile_red = 0;
	outFile_red = 0;
	errFile_red = 0;
	append = 0;
	
	amp = 0;
	
	strcpy(temp, "");
}

int checkCmd(){
	if(bi > 0){	//built in command was entered
	
		/*Check to see if built in commands are being piped with other commands*/
		if(cmd_counter > 1) {strcpy(errorMsg, "Illegal command, pipes not allowed with built in functions"); return ILLPIPE;}
		
		/*Check to see if I/O redirection are being used with built in commands*/
		if(inFile_red || outFile_red) {strcpy(errorMsg, "Illegal command, file redirection not allowed with built in functions"); return 											ILLIORED;}
		
		/*first command should be the built in command*/
		Cmd cmd = cmd_table[0];
		
		/*Check built in command*/
		if(bi == SET){
			if(cmd.num_args != 2){strcpy(errorMsg, "Wrong number of arguments, setenv requires two arguments"); return NUMARGSERR;}
			return OK;
		}
		if(bi == PRINT){
			if(cmd.num_args > 0){strcpy(errorMsg, "Warning, arguments not required and are ignored"); return OK;}
			return OK;
		}
		if(bi == UNSET){
			if(cmd.num_args != 1){strcpy(errorMsg, "Wrong number of arguments, unsetenv requires one argument"); return NUMARGSERR;}
			return OK;
		}
		if(bi == CHANGE){
			if(cmd.num_args > 1){strcpy(errorMsg, "Wrong number of arguments, cd requires one argument"); return NUMARGSERR;}
			return OK;
		}
		if(bi == AL){
			if(cmd.num_args == 0) return OK;
			if(cmd.num_args == 2) return OK;
			else{strcpy(errorMsg, "Wrong number of arguments, alias requires two arguments"); return NUMARGSERR;}
		}
		if(bi == UNAL){
			if(cmd.num_args != 1){strcpy(errorMsg, "Wrong number of arguments, unalias requires one argument"); return NUMARGSERR;}
			return OK;
		}
		if(bi == BY){
			if(cmd.num_args > 0){strcpy(errorMsg, "Warning, arguments not required and are ignored"); return OK;}
			return OK;
		}
	}
	
	else{ //built in command was not entered
		
	}
}

int execute(Cmd* cmd){
	if(bi > 0){	//built int function
		switch(bi){
			case SET:
				setenv(cmd);
				break;
				
			case PRINT:
				printenv(cmd);
				break;
				
			case UNSET:
				unsetenv(cmd);
				break;
				
			case CHANGE:
				cd(cmd);
				break;
				
			case AL:
				alias(cmd);
				break;
				
			case UNAL:
				unalias(cmd);
				break;
				
			case BY:
				bye(cmd);
				break;
				
			default:
				return CMDNOTREC;
		}
	}
	else{
		return 0;
	}	
}

void checkAlias(){
	int i;
	int j;
	Cmd* cmd;
	Alias* alias;
	for(i = 0; i < cmd_counter; i++){
		cmd = &cmd_table[i];
		for(j = 0; j < MAXALI; j++){
			alias = &alias_table[j];
			if((strcmp(cmd->cmdname, alias->name) == 0) && alias->used == 1){
				//do something to handle alias
			}
		}
	}
}







