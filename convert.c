#include <stdlib.h> // exit
#include <ctype.h> // isprint
#include <string.h> // strlen
#include <stdbool.h>
#include <sys/stat.h>

#include <assert.h>
#define PUT(s,l) write(dest,s,l);
#define PUTLIT(l) write(dest,l,sizeof(l)-1)

void convert(int dest, const char* name, int source) {
	struct stat info;
	assert(0==fstat(source,&info));
	ssize_t namelen = strlen(name);
	
	PUTLIT("const unsigned long ");
	PUT(name,namelen);
	PUTLIT("_length = 0x");
	char buf[0x100];
	ssize_t amt = snprintf(buf,0x100,"%x",info.st_size);
	PUT(buf,amt);
	PUTLIT("L;\n"
				 "const unsigned char ");
	PUT(name,namelen);
	PUTLIT("[] = \n\"");

	char* inp = mmap(NULL, info.st_size, PROT_READ, MAP_PRIVATE, source, 0);
	assert(inp != MAP_FAILED);

	char last = 0;
	bool checknext = false;
	unsigned char count = 0;
	int i = 0;
	for(;i<info.st_size;++i) {
		if(count > 60) {
			count = 0;
			PUTLIT("\"\n\"");
		}

		if(isprint(inp[i]) && inp[i] != '"') {
			PUT(inp+i,1);
			++count;
		} else {
			fputc('\\',stdout);
			++count;
			switch(inp[i]) {
#define DO(herp,derp) case herp: PUT(&derp,1); ++count; break
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
