#include "convert.h"
#include <stdlib.h> // exit, getenv

int main(void) {
	const char* name = getenv("name");
	if(name==NULL)
		exit(1);

	convert(name,0,1);
}
