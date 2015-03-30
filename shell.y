%{
	#include <stdio.h>
	#include "variable.h"
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

%token WORD

%start cmd

%token WHITESPACE

%%
cmd:		built_in
			| other

built_in:	SETENV		{printf("setenv command received\n");}
			| PRINTENV 	{printf("printenv command received\n");}
			| UNSETENV	{printf("unsetenv command received\n");}
			| CD		{printf("cd command received\n");}
			| ALIAS 	{printf("alias command received\n");}
			| UNALIAS	{printf("unalias command received\n");}
			| BYE		{printf("bye command received\n");}

other:		WORD		{fprintf(stdout, "%s command received\n", cmd);}

%%
void yyerror(char *s) {
	fprintf(stderr, "%s\n", s);
}

#include "variable.h"

int main(){
	//char* cmd[STR_LENGTH];
	yyparse();
	return 0;
}


