all:
	bison -dy shell.y
	flex shell.l

	gcc shell.h y.tab.h y.tab.c lex.yy.c shell.c -o jshell
