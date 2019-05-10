#include "first_example.h"
#include <stdio.h>
#include <assert.h>


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

	puts("And the third:");
#include "example_tres.h"
	fwrite(example_tres, example_tres_length, 1, stdout);

	assert(example_tres_length == second_example_length);
	return 0;
}
