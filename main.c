#include <stdio.h>
#include <stdlib.h> // exit
#include <ctype.h> // isprint
#include <string.h> // strlen
#include <stdbool.h>
#include <sys/stat.h>

#include <assert.h>
#define PUTLIT(l) fwrite(l,sizeof(l)-1,1,stdout)

int main(int argc, char** argv) {
	const char* name = getenv("name");
	if(name==NULL)
		exit(1);
	if(argc != 2)
		exit(2);
	FILE* src = fopen(argv[1],"rb");
	if(src==NULL)
		exit(3);
	struct stat info;
	assert(0==fstat(fileno(src),&info));

	printf("const unsigned long %s_length = 0x%xL;\n",name,info.st_size);
	printf("const unsigned char %s[] = \n\"",name);
	char last = 0;
	bool checknext = false;
	unsigned char count = 0;
	for(;;) {
		char c = getc(src);
		if(checknext) {
			if(feof(src)) {
				count += printf("%hho",last);
				break;
			} else if(count > 60 || c < '0' || c > '7') {
				count += printf("%hho",last);
			} else {
				count += printf("%03hho",last);
			}
			checknext = false;
		} else if(feof(src)) break;

		if(count > 60) {
			count = 0;
			PUTLIT("\"\n\"");
		}

		if(isprint(c) && c != '"') {
			putchar(c);
			++count;
		} else {
			fputc('\\',stdout);
			++count;
			switch(c) {
#define DO(herp,derp) case herp: fputc(derp,stdout); ++count; break
				DO(0,'0');
				DO('\\','\\');
				DO('"','"');
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
