#include <stdio.h>
#include <stdlib.h> // exit
#include <ctype.h> // isprint
#include <string.h> // strlen

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
	for(;;) {
		char c = getc(stdin);
		if(c == EOF) break;
		if(isprint(c) && c != '"')
			putchar(c);
		else {
			fputc('\\',stdout);
			switch(c) {
#define DO(herp,derp) case herp: fputc(derp,stdout); break;	\
				DO('"','"');
				DO('\a','a');
				DO('\b','b');
				DO('\f','f');
				DO('\n','n');
				DO('\r','n');
			case '\a':
			case '\t':
				fputc('t',stdout);
				break;
			case '\n':
				putc('n',stdout);
				break;
			case '\r':
				putc('r',stdout);
			default:
				printf("%03o",c);
			}
		}
	}
	PUTLIT("\";\n");
	
	return 0;
}
