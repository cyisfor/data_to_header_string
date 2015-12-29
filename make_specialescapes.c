#include <stdio.h>
int main(void) {
#define DO(herp) printf("case '\\%c':\n\tfputc('%c',stdout);\n\t++count;\n\tbreak;\n",herp,herp);
	DO('a');
	DO('b');
	DO('f');
	DO('n');
	DO('r');
	DO('t');
	DO('v');
}
