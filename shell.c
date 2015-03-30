#include "shell.h"
#include <stdio.h>

int main(){
	while(1){
		printf("jShell: ");
		yyparse();
	}
	return 0;
}
