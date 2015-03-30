all:
	bison -dy shell.y
	flex shell.l

	gcc variable.h y.tab.h y.tab.c lex.yy.c
