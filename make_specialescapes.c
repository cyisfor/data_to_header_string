#include <stdio.h>
int main(void) {
#define DO(herp) printf("case '\\%c':\n\tPUT(\"%c\",1);\n\t++count;\n\tbreak;\n",herp,herp);
	DO('a');
	DO('b');
	DO('f');
	DO('n');
	DO('r');
	DO('t');
	DO('v');
}
