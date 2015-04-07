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

/*gets local evironment variable path */
char* getLocalEnv(char* variable);

/*execute the builtin commands*/
int execute();

/*check if non-builtin command is executable*/
int executable();

/*execute non-builtin commands*/
void executeOther();

/*handel occurence of an alias*/
int handleAlias(Cmd* cmd, Alias* alias, int position);

#endif
