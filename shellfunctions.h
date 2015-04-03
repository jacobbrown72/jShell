#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include "shell.h"
#include "shellfunctions.h"
#include "shellCmds.h"

/*Print all commands that where entered*/
void printCommands();

/*reset shell global variables*/
void resetShell();

/*Check commands that where entered*/
int checkCmd();

/*execute the commands*/
int execute();

#endif
