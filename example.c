#include "first_example.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
#ifdef EXAMPLE_ONE_YAY
  puts("Yay, we defined a macro.");
  fwrite(EXAMPLE_ONE_YAY, EXAMPLE_ONE_YAY_length, 1, stdout);
  fputc('\n', stdout);
#endif

	/* INSIDE a function! */
#include "example2.txt.h"

  puts("Here's the second example:");
  fwrite(second_example, second_example_length, 1, stdout);

	#include "example_tres.
	return 0;
}
