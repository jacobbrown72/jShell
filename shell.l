%{
	#include <stdlib.h>
	#include "y.tab.h"
	#include "shell.h"
	char* str;
	int str_length;
	void yyerror(char*);
%}

%%
"setenv"		return SETENV;
"printenv"		return PRINTENV;
"unsetenv"		return UNSETENV;
"cd"			return CD;
"alias"			return ALIAS;
"unalias"		return UNALIAS;
"bye"			return BYE;

"<"				return LT;
">"				return GT;
"|"				return VERT;
"\""			return QUOTE;
"\\"			return WACK;
"&"				return AMP;
"2"				return STDERR;
"1" 			return STDOUT;
"0"				return STDIN;
[ \t]+			;
\n				return END;
[a-zA-Z0-9_:./~-]+	{str = yytext; str_length = yyleng; return WORD;}


%%

int yywrap(void) {
	return 1;
}

