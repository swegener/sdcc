/** bug-3830.c: a comparison of the address of a lower variable triggered an assertion in z80 code generation
 */

#include <testfwk.h>

int m(void) {
    char var = 'A'; // Value does not matter
    char* null = 0x0; // Value does not matter
    // Statement after if does not matter, as long
    // as it is not empty 
    if (&var != null) return 1; // z80 codegen assertion triggered on this line.
    return 0;
}

void testBug(void)
{
    ASSERT(m());
}

