#include <stdio.h>
#include <string.h>
#include "shell.h"
#include "shellfunctions.h"
#include "shellCmds.h"

int compare(char* str1, char* str2){
	int i;
	do{
		if(str1[i] != str2[i]) return 0;
	}while(str1[i] != 0);
	return 1;
}
int setenv(Cmd* cmd){
	int i;
	Env* env;
	for(i = 0; i < MAXENV; i++){
		env = &env_table[i];
		if(env->used == 0) {
			strcpy(env->variable, cmd->arguments[0]);
			strcpy(env->path, cmd->arguments[1]);
			env->used = 1;
			return OK;
		}
	}
	return SYSERR;
}

int printenv(Cmd* cmd){
	int i;
	Env* env;
	for(i = 0; i < MAXENV; i++){
		env = &env_table[i];
		if(env->used == 1) printf("%s=%s\n", env->variable, env->path);
	}
	return OK;
}

int unsetenv(Cmd* cmd){
	int i;
	Env* env;
	for(i = 0; i < MAXENV; i++){
		env = &env_table[i];
		if(strcmp(env->variable, cmd->arguments[0]) == 0) {env->used = 0; return OK;}
	}
	printf("removing environment variable failed\n");
	return SYSERR;
}

int cd(Cmd* cmd){

}

int alias(Cmd* cmd){

}

int unalias(Cmd* cmd){

}

int bye(Cmd* cmd){

}
