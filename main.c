#include <stdio.h>
#include <stdlib.h> // sizeof
#include <string.h> // strlen

struct string {
	const char* s;
	ssize_t len;
};

struct thing {
	struct string n;
	struct string v;
};

#define LIT(s) {s, (sizeof(s)-1)}
#define STR(s) {s, strlen(s)}

void out(struct string t) {
	putchar('\t');
	fwrite(t.n.s,t.n.len,1,stdout);
}

#define OUT(S) out((struct string)s)
#define COMMAND(n,v) OUT(LIT(n)); putchar('\t'); OUT(v); putchar('\n');

char buf[0x1000];

void globl(struct string n, bool needSection, string type, ssize_t align, ssize_t size) {
	COMMAND(".globl",n);
	if(needSection)
		COMMAND(".section",LIT(".rodata"));
	struct string a = {buf, 0};
	a.len = snprintf(a.buf,0x1000,"%d",align);
	COMMAND(".align",a);
	COMMAND(".type",type);
	a.len = snprintf(a.buf,"%s, %d", n.s, size);
	COMMAND(".size",a);
int main(void) {

	const char* filename = getenv("filename");
	
	struct string fn = {buf,0};
	fn.len = snprintf(buf,0x1000,"\"%s\"",filename);

	COMMAND(LIT(".file"),fn);
	OUT(LIT(".data"));
	OUT(
	
	return 0;
}
