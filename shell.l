%{
	#include <stdlib.h>
	#include "y.tab.h"
	#include "variable.h"
	extern char* cmd;
	extern int cmd_length;
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

[a-zA-Z]		{cmd = yytext; cmd_length = yyleng; return WORD;}
[ \t\n]+		return WHITESPACE;


%%

int yywrap(void) {
	return 1;
}

