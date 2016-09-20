#include "d2h_convert.h"
#include <stdlib.h> // exit, getenv
#include <string.h> // strpbrk

int main(void) {
	char* name = getenv("name");
	if(name==NULL)
		exit(1);

	{
		char* start = name;
		const char bad[] = "., ?!-";
		for(;;) {
			start = strpbrk(start,bad);
			if(start == NULL) break;
			*start = '_';
		}
	}


	d2h_convert(name,1,0);
}
