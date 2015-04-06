#ifndef CMDS_H
#define CMDS_H
#include "shell.h"
#include "shellfunctions.h"
#include "shellCmds.h"
int _setenv(Cmd* cmd);
int printenv(Cmd* cmd);
int _unsetenv(Cmd* cmd);
int cd(Cmd* cmd);
int alias(Cmd* cmd);
int unalias(Cmd* cmd);
int bye(Cmd* cmd);
#endif
