%{
	#include <stdlib.h>
	#include "y.tab.h"
	#include "shell.h"
	char* str;
	char* envstr;
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
"clear"			return CLEAR;
"bye"			return BYE;
"\""			return QUOTE;
"<"				return LT;
">"				return GT;
"|"				return VERT;
"\\"			return WACK;
"&"				return AMP;
"*"				return STAR;
"?"				return QUEST;
"2"				return STDERR;
"&1"			return STDOUT;
[ \t]+			;
\n				return END;

[a-zA-Z0-9_:./~-]*"${"[a-zA-Z0-9]+"}"[a-zA-Z0-9_:./~-]* {str = yytext; str_length = yyleng; return ENVSTR;}
[a-zA-Z0-9_:./~*?-]+	{str = yytext; str_length = yyleng; return WORD;}

%%

int yywrap(void) {
	return 1;
}

