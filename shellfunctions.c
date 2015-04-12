#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "shell.h"
#include "shellfunctions.h"
#include "shellCmds.h"


void printPrompt(){
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
	
	ret = -2;
	checkalias = -2;
	checkcommand = -2;
	
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
		strcpy(global_cmd_path[i], "");
	}
	
	/*initialize IO variables*/
	inFile_red = 0;
	outFile_red = 0;
	errFile_red = 0;
	append = 0;
	strcpy(inFile, "");
	strcpy(outFile, "");
	strcpy(errFile, "");
	iFile = -1;
	oFile = -1;
	eFile = -1;
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
	
	home_env = &env_table[0];
		strcpy(home_env->variable, "HOME");
		strcpy(home_env->path, getenv("HOME"));
		home_env->used = 1;
		
	path_env = &env_table[1];
		strcpy(path_env->variable, "PATH");
		strcpy(path_env->path, getenv("PATH"));
		path_env->used = 1;
	
	chdir(getLocalEnv("HOME"));
	clear(&cmd_table[0]);
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
	strcpy(inFile, "");
	strcpy(outFile, "");
	strcpy(errFile, "");
	iFile = -1;
	oFile = -1;
	eFile = -1;
	amp = 0;
	
	int i;
	for(i = 0; i < MAXALI; i++){
		ilist[i] = -1;
	}

	strcpy(temp, "");
	strcpy(errorMsg, "");
	strcpy(warMsg, "");
	
	ret = -2;
	checkalias = -2;
	checkcommand = -2;
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
			if(cmd.num_args < 2){strcpy(errorMsg, "Wrong number of arguments, setenv requires two arguments"); return SYSERR;}
			if(cmd.num_args > 2){strcpy(warMsg, "More then two arguments entered, extra arguments are ignored"); return WARNING;}
			return OK;
		}
		if(bi == PRINT){
			if(cmd.num_args > 0) {strcpy(warMsg, "No arguments needed, extra arguments are ignored"); return WARNING;}
			return OK;
		}
		if(bi == UNSET){
			if(cmd.num_args < 1){strcpy(errorMsg, "Wrong number of arguments, unsetenv requires one argument"); return SYSERR;}
			if(cmd.num_args > 1){strcpy(warMsg, "More then one argument entered, extra arguments are ignored"); return WARNING;}
			return OK;
		}
		if(bi == CHANGE){
			if(cmd.num_args > 1){strcpy(warMsg, "More then one argument entered, extra arguments are ignored"); return WARNING;}
			return OK;
		}
		if(bi == AL){
			if(cmd.num_args == 0) return OK;
			if(cmd.num_args == 2) return OK;
			if(cmd.num_args > 2) {strcpy(warMsg, "More then two arguments entered, extra arguments are ignored"); return WARNING;}
			else{strcpy(errorMsg, "Wrong number of arguments, alias requires two or no arguments"); return SYSERR;}
		}
		if(bi == UNAL){
			if(cmd.num_args < 1){strcpy(errorMsg, "Wrong number of arguments, unalias requires one argument"); return SYSERR;}
			if(cmd.num_args > 1){strcpy(warMsg, "More then one argument entered, extra arguments are ignored"); return WARNING;}
			return OK;
		}
		if(bi == CLR){
			if(cmd.num_args > 0) {strcpy(warMsg, "Warning, arguments not required and are ignored"); return WARNING;}
			return OK;
		}
		if(bi == BY){
			if(cmd.num_args > 0) {strcpy(warMsg, "Warning, arguments not required and are ignored"); return WARNING;}
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
			if(executable(&cmd_table[i], i) == SYSERR){sprintf(errorMsg, "Command not found: %s", cmd_table[i].cmdname); return SYSERR;}
		}
		/*Check file I/O*/
		if(inFile_red){
			iFile = open(inFile, O_RDWR);
			if(iFile < 0) {strcpy(errorMsg, "File not opened"); return SYSERR;}
		}
		
		if(outFile_red){
			if(append){
				oFile = open(outFile, O_RDWR | O_APPEND | O_CREAT, S_IRUSR | S_IWUSR);
				if(oFile < 0) {strcpy(errorMsg, "File not opened"); return SYSERR;}
			}
			else{
				oFile = open(outFile, O_RDWR | O_TRUNC | O_CREAT, S_IRUSR | S_IWUSR);
				if(oFile < 0) {strcpy(errorMsg, "File not opened"); return SYSERR;}
			}
		}
		
		if(errFile_red){
			
		}
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
				
			case CLR:
				return clear(&cmd_table[0]);
				break;

			case BY:
				return bye(&cmd_table[0]);
				break;
				
			default:
				return SYSERR;
		}
	}
	else{
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

int executable(Cmd* cmd, int index){

	char *cmdname = cmd->cmdname;

	if(cmdname[0] == '/'){
		cmdname++;
		if(access(cmdname, X_OK) == 0){
			strcpy(global_cmd_path[index], cmdname);
			return OK;
		}
	}
	else {
		char tmp[100];
		strcpy(tmp, getLocalEnv("PATH"));

		if(strcmp(tmp, "") != 0){
			char *paths = strtok(tmp, ":");

			char cmd_path[150];

			while(paths){
				strcpy(cmd_path, paths);
				strcat(cmd_path, "/");
				strcat(cmd_path, cmdname);
				if(access(cmd_path, X_OK) == 0){
					strcpy(global_cmd_path[index], cmd_path);
					return OK;
				}
				paths = strtok(NULL, ":");
			}
		}
	}
	return SYSERR;
}

int preparePipe(){
	if(cmd_counter == 1) return OK;
	
	Cmd* current_cmd;
	Cmd* previous_cmd;
	
	int i;
	for(i = 1; i < cmd_counter; i++){
		int fileds[2];
		pipe(fileds);
		if(fileds[0] == -1 || fileds[1] == -1) return SYSERR;
		//printf("reading: %d\nwriting: %d\n", fileds[0], fileds[1]);
		
		current_cmd = &cmd_table[i];
		previous_cmd = &cmd_table[i-1];
		
		previous_cmd->outfd = fileds[1];
		current_cmd->infd = fileds[0];
	}
	
	return OK;
}

void closePipe(int index){
	Cmd* cmd;
	int i;
	for(i = 0; i < cmd_counter-1; i++){
		cmd = &cmd_table[i];
		if(i != index){
			close(cmd->outfd);
			close(cmd->infd);
		}
	}
}


int executeOther(){
	Cmd* cmd;
	int pid;
	int arg_count;
	int i, j;
	int position = -1;
	char message[13];
	
	if(preparePipe() == SYSERR){strcpy(errorMsg, "Could not make pipes"); return SYSERR;}
	
	for(i = 0; i < cmd_counter; i++){
		cmd = &cmd_table[i];
		arg_count = cmd->num_args + 2;
		char* argv[arg_count];
		argv[0] = global_cmd_path[i];
		argv[arg_count-1] = NULL;
		for(j = 1; j < arg_count - 1; j++){
			argv[j] = cmd->arguments[j-1];
		}
		
		if(cmd_counter == 1) position = ONLY_ONE;
		else if(i == 0 && cmd_counter > 1) position = FIRST;
		else if(i == cmd_counter-1 && cmd_counter > 1) position = LAST;
		else position = MIDDLE;
		
		pid = fork();
		
		if(pid == 0){
			switch(position){
				case ONLY_ONE :
					if(inFile_red){
						close(0);
						dup(iFile);
						close(iFile);
					}
					if(outFile_red){
						close(1);
						dup(oFile);
						close(oFile);
					}
					break;
					
				case FIRST :
					if(inFile_red){
						close(0);
						dup(iFile);
						close(iFile);
					}
					
					close(1);
					dup(cmd->outfd);
					close(cmd->outfd);
					close(cmd->infd);
					
					//write(1, "Hello World\n", 13);
					break;
					
				case MIDDLE :
					close(0);
					dup(cmd->infd);
					close(cmd->infd);
					
					close(1);
					dup(cmd->outfd);
					close(cmd->outfd);
					
					//read(0, message, 13);
					//write(1, message, 13);
					break;
					
				case LAST :
					close(0);
					dup(cmd->infd);
					close(cmd->infd);
					close(cmd->outfd);
					
					if(outFile_red){
						close(1);
						dup(oFile);
						close(oFile);
					}
					//read(0, message, 13);
					//write(1, message, 13);
					break;
					
				default :
					printf("error occured\n"); 
			}
			closePipe(i);
			execv(global_cmd_path[i], argv);
			exit(1);
		}
		else{
			closePipe(-1);
			wait(0);
		}
	}
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


char* insertEnvVal(char *str){

	size_t end = 0, start = 0, len = strlen(str);
	char before[50], envVar[50], after[50], *envVal;
	char tmp [200] = {};

	end = len - strlen(strstr(str, "${"));
	if(end){
		strncpy(before, str, end);
		strcpy(tmp, before);
	}

	start = end + 2;
	end = len - start - strlen(strstr(str, "}"));
	strncpy(envVar, str + start, end);
	envVal = getLocalEnv(envVar);
	strcat(tmp, envVal);

	start += strlen(envVar) + 1;
	if(start){
		strcpy(after, str + start);
		strcat(tmp, after);
	}

	str = tmp;

	return str;
}
