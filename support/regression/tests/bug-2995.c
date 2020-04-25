/** bug-2995.c
    Getting the address of an object in a named address space was
    considered a read acces of that space resulting in inefficiencies
    (i.e. unnecessary calls to the function for switching address spaces).
*/

#include <testfwk.h>
#include <stdbool.h>

bool a_accessed, b_accessed;

void set_a(void)
{
  a_accessed = true;
}

void set_b(void)
{
  b_accessed = true;
}

#if !defined(PORT_HOST)
__addressmod set_a space_a;
__addressmod set_b const space_b;

space_a int i; 
space_a int *space_b j;
#endif

void testSpace(void)
{
#if !defined(PORT_HOST)
  j = &i;
  ASSERT (!a_accessed);
  ASSERT (b_accessed);
#endif
}
