#include <stdbool.h>

/* The ratio of binary / total where we start outputting a {...} style string.  */
extern float d2h_binary_threshold;
/* How many characters before forcing a new line (in the source) */
extern int d2h_max_width;
/* Do we define a macro, or define a const unsigned char[] variable? */
extern bool d2h_define_macro;
/* Is our variable static? */
extern bool d2h_static_vars;

/*
	convert source to a C array of bytes saved to dest, with the name "name"
	"name_length" is also defined, the length of that array.
*/
void d2h_convert(const char* name, int dest, int source);
