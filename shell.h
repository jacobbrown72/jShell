#ifndef VARIABLE_H
#define VARIABLE_H

/*Built in function defines*/
#define SET		1
#define PRINT	2
#define UNSET	3
#define CHANGE	4
#define AL		5
#define UNAL	6
#define BY		7

char* str;
int str_length;
int funct_value;

/*command variables*/
#define MAXCMDS 50
#define MAXARGS 50


int arg_counter;
int cmd_counter;


typedef struct command{
	char cmdname[20];		
	int bi_type;
	char arguments[MAXARGS][100];
	int num_args;
	int infd;
	int outfd;		
} Cmd;

Cmd cmd_table[MAXCMDS];

/*io variables*/
int inFile_red;
int outFile_red;
int errFile_red;
int append;

char inFile[100];
char outFile[100];
char errFile[100];

int amp;

#endif
