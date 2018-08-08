#include <stdio.h>

void DO(char herp) {
	printf("case '\\%c':\n\tPUT(\"%c\",1);\n\t++count;\n\tbreak;\n",
				 herp,
				 herp);
}

int main(void) {
	// the backslash was already outputted, now go from the raw \whatever to whatever
#define DOSTR(s,r) puts("case '\\" s "':\n\tPUT(\"" r "\",1);\n\t++count;\n\tbreak;\n");
	// these need to be backslashed AGAIN so that they can be in the string, in the code
	DOSTR("\\","\\\\");
	DOSTR("\"","\\\"");
	DO('0');
	DO('a');
	DO('b');
	DO('f');
	DO('n');
	DO('r');
//	DO('t');
	DO('v');
}
