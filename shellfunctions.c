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
}

int checkCmd(){
	if(bi > 0){	//built in command was entered

		/*Check to see if built in commands are being piped with other commands*/
		if(cmd_counter > 1) {strcpy(errorMsg, "Illegal command, pipes not allowed with built in functions"); return ILLPIPE;}

		/*Check to see if I/O redirection are being used with built in commands*/
		if(inFile_red || outFile_red) {strcpy(errorMsg, "Illegal command, file redirection not allowed with built in functions"); return ILLIORED;}
		
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
		return OK;
	}
}

int execute(){
	if(bi > 0){	//built int function
		switch(bi){
			case SET:
				set_env(&cmd_table[0]);
				break;

			case PRINT:
				print_env(&cmd_table[0]);
				break;

			case UNSET:
				unset_env(&cmd_table[0]);
				break;

			case CHANGE:
				cd(&cmd_table[0]);
				break;

			case AL:
				alias(&cmd_table[0]);
				break;

			case UNAL:
				unalias(&cmd_table[0]);
				break;

			case BY:
				bye(&cmd_table[0]);
				break;
				
			default:
				return CMDNOTREC;
		}
	}
	else{
		//call executeExt() here
		printCommands();
		return 0;
	}
}


void executeExt(Cmd* cmd){

	char tmp[100];
	strcpy(tmp, getenv("PATH"));
	char *paths = strtok(tmp, ":");

	//printf("Length: %zu, The path: %s", strlen(path), path);

	char cmd_path[150];

	while(paths){
		strcpy(cmd_path, paths);
		strcat(cmd_path, "/");
		strcat(cmd_path, cmd->cmdname);
		//printf("%s\n", cmd_path);
		if(access(cmd_path, R_OK) == 0){
			break;
		}
		paths = strtok(NULL, ":");
	}

	//char *envp[] ={ "PATH=/bin", NULL };
	//char *argv[] = {"ls", NULL};

	//printf("The path: %s\n", envp);

	//char argv[cmd->num_args+2][100];

	//strcpy(argv[0], cmd->arguments[0]);

	//int i;
	//for(i = 1; i < cmd->num_args+1; i++){
		//strcpy(argv[i], cmd->arguments[i-1]);
	//}

	//strcpy(argv[cmd->num_args+1], NULL);


	if(fork() == 0){
		//execve(argv[0], &argv[0], NULL);
		execl(cmd_path, cmd_path, NULL);
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
				if(isCircular(alias, 0)) {printf("Alias error\n"); return SYSERR;}
				handleAlias(cmd, alias, i);
			}
		}
	}
}
