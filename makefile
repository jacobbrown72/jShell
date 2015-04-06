all:
	bison -d -y shell.y
	flex shell.l

	gcc y.tab.c lex.yy.c shell.c shellfunctions.c shellCmds.c -o jshell

clean:
	rm y.tab.c lex.yy.c y.tab.h jshell
