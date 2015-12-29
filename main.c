#include <stdio.h>
#include <string.h> // strlen

int main(int argc, char** argv) {
	if(argc != 2) {
		exit(23);
	}
	const char* name = getenv("name");
	if(0!=fseek(stdin,SEEK_END,0))
		exit(82);
	long size = ftell(src);
	fseek(src,SEEK_SET,0);

	printf("const unsigned long %sSize = 0x%xL;\n",name,size);
	printf("const char %s[] = \"",name);
	for(;;) {
		char c = getc(src);
		if(c == EOF) break;
		if(isprint(c))
			putchar(c);
		else {
			printf("\\%03o",c);
		}
	}
	fputs("\";\n",stdout);
	
	return 0;
}
