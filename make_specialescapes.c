#include <stdio.h>

void DO(char herp) {
	printf("case '\\%c':\n\tPUT(\"%c\",1);\n\t++count;\n\tbreak;\n",
				 herp,
				 herp);
}

int main(void) {
	// the backslash was already outputted, now go from the raw \whatever to whatever
	DO('0');
	DO('\\');
	DO('a');
	DO('b');
	DO('f');
	DO('n');
	DO('r');
	DO('t');
	DO('v');
}
