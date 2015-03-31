#ifdef VARIABLE_H
#define VARIABLE_H

/*Built in function defines*/
#define SET		1
#define PRINT	2
#define UNSET	3
#define CHANGE	4
#define AL		5
#define UNAL	6
#define BY		7

/*command variables*/
#define MAXCMDS 50
#define MAXARGS 50

int arg_counter;
int cmd_counter;

typedef struct	comargs{
	char* args[MAXARGS];
} argtable;

typedef struct command{
	char* cmdname;		
	int bi_type;
	argtable* arguments;
	int num_args;
	int infd;
	int outfd;		
} cmd;

cmd cmd_table[MAXCMDS];

/*io variables*/

#endif
