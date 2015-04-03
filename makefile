all:
	bison -dy shell.y
	flex shell.l

	gcc shell.h shellfunctions.h shellCmds.h y.tab.h y.tab.c lex.yy.c shell.c shellfunctions.c shellCmds.c -o jshell
