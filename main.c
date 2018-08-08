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

	{
		char* maxwid = getenv("width");
		if(maxwid != NULL) {
			int maxderp = atoi(maxwid);
			if(maxderp > 0) {
				d2h_max_width = maxderp;
			}
		}
	}

	{
		char* nomacro = getenv("nomacro");
		if(nomacro) {
			d2h_define_macro = false;
		}
	}

	{
		char* staticvars = getenv("static");
		if(staticvars) {
			d2h_static_vars = true;
		}
	}
	
	d2h_convert(name,1,0);
}
