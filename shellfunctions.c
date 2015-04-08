#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "shell.h"
#include "shellfunctions.h"
#include "shellCmds.h"


void printPrompt(){
	char cwd[100];
	getcwd(cwd, sizeof(cwd));
	printf("jShell:%s$ ", cwd);
}

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
	cmd_table = (Cmd*)malloc(MAXCMDS * sizeof(Cmd));
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
	env_table = (Env*)malloc(MAXCMDS * sizeof(Env));
	for(i = 0; i < MAXENV; i++){
		env = &env_table[i];
		strcpy(env->variable, "");
		strcpy(env->path, "");
		env->used = 0;
	}
	
	/*initialize alias table*/
	alias_table = (Alias*)malloc(MAXCMDS * sizeof(Alias));
	for(i = 0; i < MAXALI; i++){
		alias = &alias_table[i];
		strcpy(alias->name, "");
		strcpy(alias->value, "");
		alias->used = 0;
		ilist[i] = -1;
	}
	
	//should initialize two environment variables here
	//	1.  "HOME"
	//	2.  "PATH"
	
	//should use home environment variable
	chdir(getenv("HOME"));
}

void resetShell(){
	/*reset arg and cmd counter*/
	arg_counter = 0;
	cmd_counter = 0;
	bi = 0;
	
	/*reset I/O variables*/
	inFile_red = 0;
	outFile_red = 0;
	errFile_red = 0;
	append = 0;

	amp = 0;
	
	int i;
	for(i = 0; i < MAXALI; i++){
		ilist[i] = -1;
	}

	strcpy(temp, "");
	strcpy(errorMsg, "");
}

int checkCmd(){
	if(bi > 0){	//built in command was entered

		/*Check to see if built in commands are being piped with other commands*/
		if(cmd_counter > 1) {strcpy(errorMsg, "Illegal command, pipes not allowed with built in functions"); return SYSERR;}

		/*Check to see if I/O redirection are being used with built in commands*/
		if(inFile_red || outFile_red) {strcpy(errorMsg, "Illegal command, file redirection not allowed with built in functions"); return SYSERR;}
		
		/*first command should be the built in command*/
		Cmd cmd = cmd_table[0];
		
		/*Check built in command*/
		if(bi == SET){
			if(cmd.num_args != 2){strcpy(errorMsg, "Wrong number of arguments, setenv requires two arguments"); return SYSERR;}
			return OK;
		}
		if(bi == PRINT){
			if(cmd.num_args > 0) strcpy(errorMsg, "Warning, arguments not required and are ignored"); 
			return OK;
		}
		if(bi == UNSET){
			if(cmd.num_args != 1){strcpy(errorMsg, "Wrong number of arguments, unsetenv requires one argument"); return SYSERR;}
			return OK;
		}
		if(bi == CHANGE){
			if(cmd.num_args > 1){strcpy(errorMsg, "Wrong number of arguments, cd requires at most one argument"); return SYSERR;}
			return OK;
		}
		if(bi == AL){
			if(cmd.num_args == 0) return OK;
			if(cmd.num_args == 2) return OK;
			else{strcpy(errorMsg, "Wrong number of arguments, alias requires two or no arguments"); return SYSERR;}
		}
		if(bi == UNAL){
			if(cmd.num_args != 1){strcpy(errorMsg, "Wrong number of arguments, unalias requires one argument"); return SYSERR;}
			return OK;
		}
		if(bi == BY){
			if(cmd.num_args > 0) strcpy(errorMsg, "Warning, arguments not required and are ignored"); 
			return OK;
		}
		else{
			strcpy(errorMsg, "Built in command not recognized");
			return SYSERR;
		}
	}
	
	else{ //built in command was not entered
		/*check to make sure commands are executable*/
		int i;
		for(i = 0; i < cmd_counter; i++){
			if(executable(&cmd_table[i]) == SYSERR){strcpy(errorMsg, "Command not found"); return SYSERR;}
		}
		/*Check file I/O*/
		return OK;
	}
}

int execute(){
	if(bi > 0){	//built int function
		switch(bi){
			case SET:
				return set_env(&cmd_table[0]);
				break;

			case PRINT:
				return print_env(&cmd_table[0]);
				break;

			case UNSET:
				return unset_env(&cmd_table[0]);
				break;

			case CHANGE:
				return cd(&cmd_table[0]);
				break;

			case AL:
				return alias(&cmd_table[0]);
				break;

			case UNAL:
				return unalias(&cmd_table[0]);
				break;

			case BY:
				return bye(&cmd_table[0]);
				break;
				
			default:
				return SYSERR;
		}
	}
	else{
		//printCommands();
		executeOther();
		return OK;
	}
}

char* getLocalEnv(char * variable){
	int i;
	for(i = 0; i < MAXENV; i++){
		Env *env = &env_table[i];
		if(strcmp(env->variable, variable) == 0){
			return env->path;//found
		}
	}
	return ""; //not found
}

int executable(Cmd* cmd){

	/*need to check if cmdname contains a "/"*/
	/*if cmdname contains a "/" we search the path of the cmd name, not the PATH variable*/

	char tmp[100];
	strcpy(tmp, getenv("PATH"));

	if(strcmp(tmp, "") != 0){
		char *paths = strtok(tmp, ":");

		char cmd_path[150];

		while(paths){
			strcpy(cmd_path, paths);
			strcat(cmd_path, "/");
			strcat(cmd_path, cmd->cmdname);
			if(access(cmd_path, X_OK) == 0){
				other_cmd_path = cmd_path;
				return OK; //file found on path
			}
			paths = strtok(NULL, ":");
		}
		strcpy(errorMsg, "Command not found");
		return SYSERR;
	}
}

void executeOther(){
	Cmd *cmd = &cmd_table[0];

	int i, arg_num = arg_counter+2;

	char * argv[arg_num];

	/* create argument array */
	for(i = 0; i < arg_num; i++){
		if(i == 0){
			argv[i] = other_cmd_path;
		} else if (i == (arg_num - 1)) {
			argv[i] = NULL;
		}else{
			argv[i] = cmd->arguments[i-1];
		}
	}

	if(fork() == 0){
		execve(other_cmd_path, argv, NULL);
		//execl(other_cmd_path, other_cmd_path, NULL);
		fprintf(stderr, "Command not found: %s\n", cmd->cmdname);
		exit(1);
	}
	wait();
}


int findAlias(Alias* alias){
	Alias* aptr;
	int i;
	for(i = 0; i < MAXALI; i++){
		aptr = &alias_table[i];
		if((strcmp(alias->value, aptr->name) == 0) && aptr->used == 1) return i;
	}
	return -1;
}

int isCircular(Alias* alias, int i){
	int c;
	int x;
	
	c = findAlias(alias);
	if(c == -1) return 0;
	
	for	(x = 0; x <= i; x++){
		if(ilist[x] == c) return 1;
	}
	
	ilist[i] = c;
	i++;
	
	isCircular(&alias_table[c], i);
}

int handleAlias(Cmd* cmd, Alias* alias, int position){
	char cmd_word[50];

	int i;
	int old_cmd_count = cmd_counter;
	cmd_counter = 0;
	Cmd* cmd_table_old = cmd_table; 
	Cmd* temp;
	cmd_table = (Cmd*)malloc(100 * sizeof(Cmd));
	
	strcpy(cmd_word, alias->value);
	strcat(cmd_word, "\n");

	FILE* cmd_file = fopen("cmd_file.txt", "w");
	fputs(cmd_word, cmd_file);
	fclose(cmd_file);
	
	cmd_file = fopen("cmd_file.txt", "r");
	yyin = cmd_file;
	yyparse();
	fclose(cmd_file);
	yyin = stdin;
	
	for(i = old_cmd_count-1; i > position; i--){
		cmd_table_old[i+cmd_counter-1] = cmd_table_old[i];
	}
	for(i = 0; i < cmd_counter; i++){
		cmd_table_old[position+i] = cmd_table[i];
	}
	
	temp = cmd_table;
	cmd_table = cmd_table_old;
	cmd_table_old = temp;
	//free(cmd_table_old);		//this might cause memory issues, if sample imputs are small it shouldn't be a big deal
	cmd_counter = old_cmd_count + cmd_counter - 1;
		
	checkAlias();
	
	return OK;
}

int checkAlias(){
	int i;
	int j;
	Cmd* cmd;
	Alias* alias;
	for(i = 0; i < cmd_counter; i++){
		cmd = &cmd_table[i];
		for(j = 0; j < MAXALI; j++){
			alias = &alias_table[j];
			if((strcmp(cmd->cmdname, alias->name) == 0) && alias->used == 1){
				if(isCircular(alias, 0)) {strcpy(errorMsg, "Recursive Alias error"); return SYSERR;}
				handleAlias(cmd, alias, i);
			}
		}
	}
	return OK;
}
