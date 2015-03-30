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

'<'				return LT;
'>'				return GT;
'|'				return VERT;
'\"'			return QUOTE;
'\'				return WACK;
'&'				return AMP;

[a-zA-Z]+		{str = yytext; str_length = yyleng; return WORD;}
[ \t]+			return WHITESPACE;


%%

int yywrap(void) {
	return 1;
}

