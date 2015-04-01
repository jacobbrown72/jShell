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
%token STDERR
%token STDOUT
%token STDIN
%token END
%token WORD
%token WHITESPACE
%start line



%%

line:		END					{YYACCEPT;}
			| cmds END  		{YYACCEPT;}
			| cmds AMP END		{YYACCEPT;}
			| cmds redir END	{YYACCEPT;}
			| cmds redir AMP END{YYACCEPT;}
			
cmds:		cmd.args
			| cmd.args VERT cmds
			
cmd.args:	cmd args

cmd:		SETENV			{
								arg_counter = 0;
								Cmd new_cmd;
								strcpy(new_cmd.cmdname, "setenv");
								new_cmd.bi_type = SET;
								new_cmd.num_args = 0;
								cmd_table[cmd_counter] = new_cmd;
								cmd_counter++;
							}

			| PRINTENV		{
								arg_counter = 0;
								Cmd new_cmd;
								strcpy(new_cmd.cmdname, "printenv");
								new_cmd.bi_type = PRINT;
								new_cmd.num_args = 0;
								cmd_table[cmd_counter] = new_cmd;
								cmd_counter++;
							}
			
			| UNSETENV		{
								arg_counter = 0;
								Cmd new_cmd;
								strcpy(new_cmd.cmdname, "unsetenv");
								new_cmd.bi_type = UNSET;
								new_cmd.num_args = 0;
								cmd_table[cmd_counter] = new_cmd;
								cmd_counter++;
							}
			
			| CD			{
								arg_counter = 0;
								Cmd new_cmd;
								strcpy(new_cmd.cmdname, "cd");
								new_cmd.bi_type = CHANGE;
								new_cmd.num_args = 0;
								cmd_table[cmd_counter] = new_cmd;
								cmd_counter++;
							}
			
			| ALIAS			{
								arg_counter = 0;
								Cmd new_cmd;
								strcpy(new_cmd.cmdname, "alias");
								new_cmd.bi_type = AL;
								new_cmd.num_args = 0;
								cmd_table[cmd_counter] = new_cmd;
								cmd_counter++;
							}
			
			| UNALIAS		{
								arg_counter = 0;
								Cmd new_cmd;
								strcpy(new_cmd.cmdname, "unalias");
								new_cmd.bi_type = UNAL;
								new_cmd.num_args = 0;
								cmd_table[cmd_counter] = new_cmd;
								cmd_counter++;
							}
			
			| BYE			{
								arg_counter = 0;
								Cmd new_cmd;
								strcpy(new_cmd.cmdname, "bye");
								new_cmd.bi_type = BY;
								new_cmd.num_args = 0;
								cmd_table[cmd_counter] = new_cmd;
								cmd_counter++;
							}
							
			| WORD			{
								arg_counter = 0;
								Cmd new_cmd;
								strcpy(new_cmd.cmdname, str);
								new_cmd.bi_type = 0;
								new_cmd.num_args = 0;
								cmd_table[cmd_counter] = new_cmd;
								cmd_counter++;
							}

args:		/*no arguments*/	
			| args WORD		{
								Cmd* my_cmd = &cmd_table[cmd_counter-1];
								strcpy(my_cmd->arguments[arg_counter], str);
								arg_counter++;
								my_cmd->num_args = arg_counter;
							}

redir:		input_red output_red err_red
			| input_red err_red output_red
			| output_red input_red err_red
			| output_red err_red input_red
			| err_red input_red output_red
			| err_red output_red input_red
			
input_red:	LT file

output_red:	GT file
			| GT GT file

err_red:	STDERR GT file

file:		STDIN
			| STDOUT
			| STDERR
			| WORD
		

%%
void yyerror(char *s) {
	fprintf(stderr, "%s\n", s);
}





