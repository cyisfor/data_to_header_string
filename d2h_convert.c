#include <stdlib.h> // exit
#include <ctype.h> // isprint
#include <string.h> // strlen
#include <stdbool.h>
#include <sys/stat.h>
#include <unistd.h> // write
#include <sys/mman.h>
#include <stdio.h> //

#include <assert.h>
#define PUT(s,l) if(write(dest,s,l));
#define PUTLIT(l) if(write(dest,l,sizeof(l)-1));

static
unsigned char digits[0x10] = "0123456789abcdef";


int d2h_max_width = 90;

bool d2h_define_macro = true;
bool d2h_static_vars = false;

static size_t itoa(size_t left, unsigned char* buf, int maxlen) {

	ssize_t amt = 0;
	while(left) {
		unsigned char digit = left & 0xF;
		left = left >> 4;
		buf[amt++] = digits[digit];
		assert(amt < maxlen);
	}
	// now reverse it...
	int i = 0;
	for(i=0;i<amt>>1;++i) {
		unsigned char temp = buf[i];
		buf[i] = buf[amt-i-1];
		buf[amt-i-1] = temp;
	}
	return amt;
}

static size_t itoa9(size_t left, unsigned char* buf, int maxlen) {
	static
		unsigned char digits[0x10] = "0123456789";
	ssize_t amt = 0;
	while(left) {
		unsigned char digit = left % 10;
		left = left / 10;
		buf[amt++] = digits[digit];
		assert(amt < maxlen);
	}
	// now reverse it...
	int i = 0;
	for(i=0;i<amt>>1;++i) {
		unsigned char temp = buf[i];
		buf[i] = buf[amt-i-1];
		buf[amt-i-1] = temp;
	}
	return amt;
}

static
void output_escaped(int dest, const unsigned char* inp, size_t size) {
	bool needopenquote = true;
	void checkopenquote(void) {
		if(needopenquote) {
			if(d2h_define_macro) {
				PUTLIT(" \\\n\"");
			} else {
				PUTLIT("\n\"");
			}
			needopenquote = false;
		}
	}

	unsigned char count = 0;
	int i;
	for(i=0;i<size;++i) {
		if((isprint(inp[i]) && inp[i] != '"') || inp[i] == '\t') {
			checkopenquote();
			PUT(inp+i,1);
			++count;
		} else if(inp[i] == '\n') {
			// have newlines make newlines to keep it pretty
			checkopenquote();
			PUTLIT("\\n\"");
			needopenquote = true;
			count = 0;
		} else {
			++count;
			// we have to escape SOMEthing!
			checkopenquote();
			PUTLIT("\\");
			unsigned char c = inp[i];
			switch(c) {

#include "specialescapes.c"

			default:
				if(c >= 0100) {
					// we're cool
					PUT(&digits[c >> 6 & 007],1);
					PUT(&digits[c >> 3 & 007],1);
					PUT(&digits[c >> 0 & 007],1);
				} else {
					bool needzeroes = !(i + 1 == size ||
															count > 60 ||
															inp[i+1] < '0' ||
															inp[i+1] > '7');
					// we can skimp on zeroes if we're ending a quote, or at the end of the file, or the next character isn't 0-7.
					if(c < 010) {
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
	if(needopenquote == false) {
		PUTLIT("\"");
	}
	if(d2h_define_macro) {
		PUTLIT("\n");
	} else {
		PUTLIT(";\n");
	}
}

static
void output_binary(int dest, const unsigned char* inp, size_t size) {
	if(d2h_define_macro) {
		PUTLIT("(const unsigned char*)");
	}
	PUTLIT("{\n");
	int i;
	for(i=0;i<size;++i) {
		if(i == 0) {
		} else {
			if((i+1) % d2h_max_width == 0) {
				if(d2h_define_macro) {
					PUTLIT(", \\\n");
				} else {
					PUTLIT(",\n");
				}
			} else {
				PUTLIT(", ");
			}
		}

		if((isprint(inp[i]) && inp[i] != '\'')) {
			PUTLIT("'");
			PUT(inp+i,1);
			PUTLIT("'");
		} else {
			char buf[0x100];
			size_t amt = itoa9(inp[i], buf, 0x100);
			PUT(buf, amt);
		}
	}
	PUTLIT("\n}");
	if(!d2h_define_macro) {
		PUTLIT(";");
	}
	PUTLIT("\n");
}

void d2h_convert(const unsigned char* name, int dest, int source) {
	struct stat info;
	unsigned char buf[0x100];
	assert(0==fstat(source,&info));
	ssize_t namelen = strlen(name);

	if(d2h_define_macro) {
		PUTLIT("#define ");
		PUT(name,namelen);
		PUTLIT("_length 0x");
		size_t amt = itoa(info.st_size, buf, 0x100);
		PUT(buf,amt);
		PUTLIT("L\n");
		PUTLIT("#define ");
		PUT(name, namelen);
	} else {
		if(d2h_static_vars) {
			PUTLIT("static\n");
		}
		PUTLIT("const unsigned long ");
		PUT(name,namelen);
		PUTLIT("_length = 0x");
		size_t amt = itoa(info.st_size, buf, 0x100);
		PUT(buf,amt);
		PUTLIT("L;\n");

		if(d2h_static_vars) {
			PUTLIT("static\n");
		}
		PUTLIT("const unsigned char ");
		PUT(name,namelen);
		PUTLIT("[] = ");
	}

	unsigned char* inp = mmap(NULL, info.st_size, PROT_READ, MAP_PRIVATE, source, 0);
	assert(inp != MAP_FAILED);

	/* first, scan to see which format is best... */

	unsigned char last = 0;
	bool checknext = false;
	int binarychars = 0;
	int i;
	for(i=0;i<info.st_size;++i) {
		if((isprint(inp[i]) && inp[i] != '"') || inp[i] == ' ') {
			continue;
		}
		++binarychars;
	}
	fprintf(stderr, "Umm %d %ld %lf\n",
					binarychars, info.st_size, 100.0 * binarychars / info.st_size);


	if(binarychars > info.st_size * 1 / 2) {
		output_binary(dest, inp, info.st_size);
	} else {
		output_escaped(dest, inp, info.st_size);
	}
	close(source);
	//close(dest); can't write multiple strings this way!
}
