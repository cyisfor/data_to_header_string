#include <stdio.h>
#include <stdlib.h> // exit
#include <ctype.h> // isprint
#include <string.h> // strlen
#include <stdbool.h>
#define PUTLIT(l) fwrite(l,sizeof(l),1,stdout)

int main(int argc, char** argv) {
	const char* name = getenv("name");
	if(name==NULL)
		exit(23);
	if(0!=fseek(stdin,SEEK_END,0))
		exit(82);
	long size = ftell(stdin);
	fseek(stdin,SEEK_SET,0);

	printf("const unsigned long %sSize = 0x%xL;\n",name,size);
	printf("const unsigned char %s[] = \"",name);
	char last;
	bool checknext = false;
	for(;;) {
		char c = getc(stdin);
		if(checknext) {
			if(c==EOF) {
				printf("%o",last);
				break;
			} else if(c < '0' || c > '7') {
				printf("%o",last);
			} else {
				printf(">%03o<",last);
				putchar(c);
				exit(3);
			}
			checknext = false;
		}

		if(c == EOF) break;
		if(isprint(c) && c != '"')
			putchar(c);
		else {
			fputc('\\',stdout);
			switch(c) {
			case 0: fputc('0',stdout);
			case '\\': fputc('\\',stdout);
			case '"': fputc('"',stdout);
#include "specialescapes.c"
			default:
				checknext = true;
				last = c;
			};
		}
	}
	PUTLIT("\";\n");
	
	return 0;
}
