#include <stdlib.h> // exit
#include <ctype.h> // isprint
#include <string.h> // strlen
#include <stdbool.h>
#include <sys/stat.h>
#include <unistd.h> // write
#include <sys/mman.h>

#include <assert.h>
#define PUT(s,l) write(dest,s,l);
#define PUTLIT(l) write(dest,l,sizeof(l)-1)

static
char digits[0x10] = "0123456789abcdef";

int d2h_max_width = 90;

void d2h_convert(const char* name, int dest, int source) {
	struct stat info;
	char buf[0x100];
	assert(0==fstat(source,&info));
	ssize_t namelen = strlen(name);

	PUTLIT("const unsigned long ");
	PUT(name,namelen);
	PUTLIT("_length = 0x");
	size_t left = info.st_size;
	ssize_t amt = 0;
	while(left) {
		char digit = left & 0xF;
		left = left >> 4;
		buf[amt++] = digits[digit];
		assert(amt < 0x100);
	}
	// now reverse it...
	int i = 0;
	for(i=0;i<amt>>1;++i) {
		char temp = buf[i];
		buf[i] = buf[amt-i-1];
		buf[amt-i-1] = temp;
	}
	PUT(buf,amt);
	PUTLIT("L;\n"
				 "const unsigned char ");
	PUT(name,namelen);
	PUTLIT("[] = \n");

	bool needopenquote = true;
	void checkopenquote(void) {
		if(needopenquote) {
			PUTLIT("\"");
			needopenquote = false;
		}
	}
	char* inp = mmap(NULL, info.st_size, PROT_READ, MAP_PRIVATE, source, 0);
	assert(inp != MAP_FAILED);

	char last = 0;
	bool checknext = false;
	unsigned char count = 0;
	for(i=0;i<info.st_size;++i) {
		if(count > d2h_max_width) {
			count = 0;
			if(needopenquote == false) {
				PUTLIT("\"\n");
				needopenquote = true;
			}
		}

		if((isprint(inp[i]) && inp[i] != '"') || inp[i] == '\t') {
			checkopenquote();
			PUT(inp+i,1);
			++count;
		} else if(inp[i] == '\n') {
			// have newlines make newlines to keep it pretty
			checkopenquote();
			PUTLIT("\\n\"\n");
			needopenquote = true;
			count = 0;
		} else {
			++count;
			// we have to escape SOMEthing!
			checkopenquote();
			PUTLIT("\\");
			char c = inp[i];
			switch(c) {
				
#include "specialescapes.c"
				
			default:				
				if(c > 0100) {
					// we're cool
					PUT(&digits[c >> 6 & 007],1);
					PUT(&digits[c >> 3 & 007],1);
					PUT(&digits[c >> 0 & 007],1);
				} else {
					bool needzeroes = !(i + 1 == info.st_size ||
															count > 60 ||
															inp[i+1] < '0' ||
															inp[i+1] > '7');
					// we can skimp on zeroes if we're ending a quote, or at the end of the file, or the next character isn't 0-7.
					if(c > 010) {
						// we need 1 zero
						if(needzeroes)
							PUT("0",1);
						PUT(&digits[c >> 3 & 007],1);
						PUT(&digits[c & 007],1);
					} else {
						// we need 2 zeroes
						if(needzeroes)
							PUT("00",2);
						PUT(&digits[c & 007],1);
					}
				}
			};
		}
	}
	close(source);
	if(needopenquote == false) {
		PUTLIT("\"");
	}
	PUTLIT(";\n");
	//close(dest); can't write multiple strings this way!
}
