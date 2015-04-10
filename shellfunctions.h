#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include "shell.h"
#include "shellfunctions.h"
#include "shellCmds.h"

/*initialize variables for shell*/
void initShell();

/*prints the command prompt*/
void printPrompt();

/*Print all commands that where entered*/
void printCommands();

/*reset shell global variables*/
void resetShell();

/*Check commands that where entered*/
int checkCmd();

/*check for and exapand aliases found in the command*/
int checkAlias();

/*execute the builtin commands*/
int execute();

/*execute the external commands*/
int executeOther();

/*handel occurence of an alias*/
int handleAlias(Cmd* cmd, Alias* alias, int position);

/*determine if there is a recursive alias*/
int isCircular(Alias* alias, int i);

/*find index of alias in alias_table*/
int findAlias(Alias* alias);

int executable(Cmd* cmd, int index);

char* getLocalEnv(char * variable);


#endif
