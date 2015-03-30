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



%start cmd

%token WHITESPACE

%%
		

%%
void yyerror(char *s) {
	fprintf(stderr, "%s\n", s);
}




