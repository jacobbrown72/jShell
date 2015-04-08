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
%token CLEAR

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
			| cmds AMP END		{amp = 1; YYACCEPT;}
			| cmds redir END	{YYACCEPT;}
			| cmds redir AMP END{amp = 1; YYACCEPT;}

cmds:		cmd.args
			| cmd.args VERT cmds
			
cmd.args:	cmd args

cmd:		SETENV			{
								arg_counter = 0;
								Cmd* new_cmd = &cmd_table[cmd_counter];
								strcpy(new_cmd->cmdname, "setenv");
								new_cmd->bi_type = SET;
								new_cmd->num_args = 0;
								cmd_counter++;
								bi = SET;
							}

			| PRINTENV		{
								arg_counter = 0;
								Cmd* new_cmd = &cmd_table[cmd_counter];
								strcpy(new_cmd->cmdname, "printenv");
								new_cmd->bi_type = PRINT;
								new_cmd->num_args = 0;
								cmd_counter++;
								bi = PRINT;
							}
			
			| UNSETENV		{
								arg_counter = 0;
								Cmd* new_cmd = &cmd_table[cmd_counter];
								strcpy(new_cmd->cmdname, "unsetenv");
								new_cmd->bi_type = UNSET;
								new_cmd->num_args = 0;
								cmd_counter++;
								bi = UNSET;
							}
			
			| CD			{
								arg_counter = 0;
								Cmd* new_cmd = &cmd_table[cmd_counter];
								strcpy(new_cmd->cmdname, "cd");
								new_cmd->bi_type = CHANGE;
								new_cmd->num_args = 0;
								cmd_counter++;
								bi = CHANGE;
							}
			
			| ALIAS			{
								arg_counter = 0;
								Cmd* new_cmd = &cmd_table[cmd_counter];
								strcpy(new_cmd->cmdname, "alias");
								new_cmd->bi_type = AL;
								new_cmd->num_args = 0;
								cmd_counter++;
								bi = AL;
							}
			
			| UNALIAS		{
								arg_counter = 0;
								Cmd* new_cmd= &cmd_table[cmd_counter];
								strcpy(new_cmd->cmdname, "unalias");
								new_cmd->bi_type = UNAL;
								new_cmd->num_args = 0;
								cmd_counter++;
								bi = UNAL;
							}
							
			| CLEAR			{
								arg_counter = 0;
								Cmd* new_cmd = &cmd_table[cmd_counter];
								strcpy(new_cmd->cmdname, "clear");
								new_cmd->bi_type = CLR;
								new_cmd->num_args = 0;
								cmd_counter++;
								bi = CLR;
							}
			
			| BYE			{
								arg_counter = 0;
								Cmd* new_cmd = &cmd_table[cmd_counter];
								strcpy(new_cmd->cmdname, "bye");
								new_cmd->bi_type = BY;
								new_cmd->num_args = 0;
								cmd_counter++;
								bi = BY;
							}
							
			| WORD			{
								arg_counter = 0;
								Cmd* new_cmd = &cmd_table[cmd_counter];
								strcpy(new_cmd->cmdname, str);
								new_cmd->bi_type = 0;
								new_cmd->num_args = 0;
								cmd_counter++;
							}

args:		/*no arguments*/	
			| args WORD		{
								Cmd* my_cmd = &cmd_table[cmd_counter-1];
								strcpy(my_cmd->arguments[arg_counter], str);
								arg_counter++;
								my_cmd->num_args = arg_counter;
							}
			| args quote	{
								Cmd* my_cmd = &cmd_table[cmd_counter-1];
								strcpy(my_cmd->arguments[arg_counter], temp);
								arg_counter++;
								my_cmd->num_args = arg_counter;
							}
							
quote:		QUOTE words QUOTE
			| QUOTE QUOTE	{strcpy(temp, "");}
			
words:		WORD			{
								strcat(temp, str);
							}
			| words WORD	{
								strcat(temp, " ");
								strcat(temp, str);
							}
							
			| words VERT WORD 	{
									strcat(temp, " | ");
									strcat(temp, str);
								}

redir:		input_red output_red err_red
			| input_red err_red output_red
			| output_red input_red err_red
			| output_red err_red input_red
			| err_red input_red output_red
			| err_red output_red input_red
			
			| input_red output_red
			| output_red input_red
			
			| input_red err_red 
			| err_red input_red
			
			| output_red err_red
			| err_red output_red
			
			| input_red
			| output_red
			| err_red
			
input_red:	LT infile		{
								inFile_red = 1;
							}

output_red:	GT outfile		{
								outFile_red = 1;
							}
								
			| GT GT outfile	{
								outFile_red = 1;
								append = 1;
							}

err_red:	STDERR GT errfile	{
									errFile_red = 1;
								}

infile:		WORD			{
								strcpy(inFile, str);
							}
							
outfile:	WORD			{
								strcpy(outFile, str);
							}
							
errfile:	WORD			{
								strcpy(errFile, str);
							}

%%
void yyerror(char *s) {
	fprintf(stderr, "%s\n", s);
}
