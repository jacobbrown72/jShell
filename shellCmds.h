#ifndef CMDS_H
#define CMDS_H
#include "shell.h"

int set_env(Cmd* cmd);
int print_env(Cmd* cmd);
int unset_env(Cmd* cmd);
int cd(Cmd* cmd);
int alias(Cmd* cmd);
int unalias(Cmd* cmd);
int bye(Cmd* cmd);

#endif
