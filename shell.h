#ifndef SHELL_H
#define SHELL_H

#include <stdlib.h>
#include <stdio.h>


/*Built in function defines*/
#define SET		1
#define PRINT	2
#define UNSET	3
#define CHANGE	4
#define AL		5
#define UNAL	6
#define BY		7
#define CLR		8

/*shell error declerations*/
#define CLOSE			-1		//close terminal
#define SYSERR			0		//error occured
#define OK 				1		//command is ok
#define WARNING			2

/*function position*/
#define ONLY_ONE		0
#define FIRST			1
#define LAST			2
#define MIDDLE			3

/*File descriptors*/
#define READ			0
#define WRITE			1
	
extern FILE* yyin;

char errorMsg[100];
char warMsg[100];
char temp[100];
char cwd[100];

char* str;
int str_length;

int checkalias;
int checkcommand;
int ret;

/*command variables*/
#define MAXCMDS 50
#define MAXARGS 50

int arg_counter;
int cmd_counter;
int bi;

typedef struct command{
	char cmdname[20];
	int bi_type;
	char arguments[MAXARGS][100];
	int num_args;
	int infd;
	int outfd;
} Cmd;

Cmd* cmd_table;
char global_cmd_path[MAXCMDS][100];

/*io variables*/
int inFile_red;
int outFile_red;
int errFile_red;
int append;

int iFile;
int oFile;
int eFile;

char inFile[100];
char outFile[100];
char errFile[100];

int amp;

/*environment variables*/
#define MAXENV 100
typedef struct env{
	char variable[100];
	char path[100];
	int used;
} Env;

Env* home_env;
Env* path_env;

Env* env_table;

/*alias variables*/
#define MAXALI 100
typedef struct alias{
	char name[100];
	char value[100];
	int used;
} Alias;

Alias* alias_table;
int ilist[MAXALI];

#endif









