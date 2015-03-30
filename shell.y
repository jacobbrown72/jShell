%{
	#include <stdio.h>
	#include "shell.h"
	int yylex(void);
	void  yyerror(char*);
	char* cmd;
	int cmd_length;
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
%token END		// \n

%token WORD

%start cmd

%token WHITESPACE

%error-verbose

%%
cmd:		built_in 
			| other 

built_in:	SETENV		{printf("setenv command received\n"); YYACCEPT;}
			| PRINTENV 	{printf("printenv command received\n"); YYACCEPT;}
			| UNSETENV	{printf("unsetenv command received\n"); YYACCEPT;}
			| CD		{printf("cd command received\n"); YYACCEPT;}
			| ALIAS 	{printf("alias command received\n"); YYACCEPT;}
			| UNALIAS	{printf("unalias command received\n"); YYACCEPT;}
			| BYE		{printf("bye command received\n"); YYACCEPT;}

other:		WORD		{printf("%s command received\n", cmd); YYACCEPT;}

%%
void yyerror(char *s) {
	fprintf(stderr, "%s\n", s);
}




