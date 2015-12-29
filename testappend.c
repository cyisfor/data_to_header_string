#include <stdio.h>
#define UM(a) printf("%x %x\n",'\ ## a ## ',' ## a ## ');

int main(void) {
	UM(t);
}
