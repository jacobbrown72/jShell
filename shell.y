%{
	#include <stdio.h>
	#include <stdlib.h>
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

%start line

%token WHITESPACE

%%

line:		cmds io_red AMP
			| cmds AMP
			| cmds io_red
			| cmds
		
cmds: 		cmd.args
			| cmd.args VERT cmds
		
cmd.args:	cmd
			| cmd args

cmd:		SETENV
			| PRINTENV
			| UNSETENV
			| CD
			| ALIAS
			| UNALIAS
			| BYE
			| WORD

args:		WORD
			| WORD args

io_red:
		

%%
void yyerror(char *s) {
	fprintf(stderr, "%s\n", s);
}




