#include <stdio.h>
#include <string.h>
#include "shell.h"
#include "shellfunctions.h"
#include "shellCmds.h"

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
	return SYSERR;
}

int cd(Cmd* cmd){
	if(cmd->num_args == 0){	//return home
		
	}
	else{ 					//go to new directory
	
	}
}

int alias(Cmd* cmd){
	int i;
	Alias* alias;
	if(cmd->num_args == 0){//print aliases
		for(i = 0; i < MAXALI; i++){
			alias = &alias_table[i];
			if(alias->used == 1) printf("%s=%s\n", alias->name, alias->value);
		}
		return OK;
	}
	else{	//create a new alias
		for(i = 0; i < MAXALI; i++){
			alias = &alias_table[i];
			if(alias->used == 0){
				strcpy(alias->name, cmd->arguments[0]);
				strcpy(alias->value, cmd->arguments[1]);
				alias->used = 1;
				return OK;
			}
		}
		return SYSERR;
	}
}

int unalias(Cmd* cmd){
	int i;
	Alias* alias;
	for(i = 0; i < MAXALI; i++){
		alias = &alias_table[i];
		if(strcmp(alias->name, cmd->arguments[0]) == 0){alias->used = 0; return OK;}
	}
	return SYSERR;
}

int bye(Cmd* cmd){
	return CLOSE;
}
