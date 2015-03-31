%{
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include "shell.h"
	int yylex(void);
	void  yyerror(char*);
	char* str;
	int str_length;
%}
/*token definitions for built in functions*/
%token SETENV
%token PRINTENV
%token UNSETENV
%token CD
%token ALIAS
%token UNALIAS
%token BYE

/*token definitions for metacharacters*/
%token LT		// <
%token GT		// >
%token VERT		// |
%token QUOTE	// "
%token WACK		// "\"	
%token AMP		// &

%token WORD

%start cmd

%token WHITESPACE

%%
/*
line:		cmds io_red AMP
			| cmds AMP
			| cmds io_red
			| cmds

		
cmds: 		cmds VERT cmd.args
			| cmd.args 

		
cmd.args:	cmd args
			| cmd 
*/

cmd:		SETENV			{
								Cmd new_cmd;
								new_cmd.cmdname = "setenv";
								new_cmd.bi_type = SET;
								new_cmd.num_args = 0;
								cmd_table[cmd_counter] = new_cmd;
								cmd_counter++;
								YYACCEPT;
							}
							
			| PRINTENV		{
								Cmd new_cmd;
								new_cmd.cmdname = "printenv";
								new_cmd.bi_type = PRINT;
								new_cmd.num_args = 0;
								cmd_table[cmd_counter] = new_cmd;
								cmd_counter++;
								YYACCEPT;
							}
							
			| UNSETENV		{
								Cmd new_cmd;
								new_cmd.cmdname = "unsetenv";
								new_cmd.bi_type = UNSET;
								new_cmd.num_args = 0;
								cmd_table[cmd_counter] = new_cmd;
								cmd_counter++;
								YYACCEPT;
							}
							
			| CD			{
								Cmd new_cmd;
								new_cmd.cmdname = "cd";
								new_cmd.bi_type = CHANGE;
								new_cmd.num_args = 0;
								cmd_table[cmd_counter] = new_cmd;
								cmd_counter++;
								YYACCEPT;
							}
							
			| ALIAS			{
								Cmd new_cmd;
								new_cmd.cmdname = "alias";
								new_cmd.bi_type = AL;
								new_cmd.num_args = 0;
								cmd_table[cmd_counter] = new_cmd;
								cmd_counter++;
								YYACCEPT;
							}
							
			| UNALIAS		{
								Cmd new_cmd;
								new_cmd.cmdname = "unalias";
								new_cmd.bi_type = UNAL;
								new_cmd.num_args = 0;
								cmd_table[cmd_counter] = new_cmd;
								cmd_counter++;
								YYACCEPT;
							}
							
			| BYE			{
								Cmd new_cmd;
								new_cmd.cmdname = "bye";
								new_cmd.bi_type = BY;
								new_cmd.num_args = 0;
								cmd_table[cmd_counter] = new_cmd;
								cmd_counter++;
								YYACCEPT;
							}
							
			| WORD			{
								Cmd new_cmd;
								strcpy(cmdname_table[cmd_counter], str);
								new_cmd.cmdname = cmdname_table[cmd_counter];
								new_cmd.bi_type = 0;
								new_cmd.num_args = 0;
								cmd_table[cmd_counter] = new_cmd;
								cmd_counter++;
								YYACCEPT;
							}
/*
args:		args WORD
			| WORD 
*/

//io_red:
		

%%
void yyerror(char *s) {
	fprintf(stderr, "%s\n", s);
}





