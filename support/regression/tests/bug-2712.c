/*
   bug-2458.c
   In backends that support 16x16->32 multiplication, there awas a bug when
   operands to such a multiplication were used in multiple such multiplications.
*/

#include <testfwk.h>

int dummy(int x)
{
   return x;
}

void dummy2(long x)
{
   ASSERT(x == 42 * 42 || x == -42 * 42);
}

void testBug(void)
{
   int i, j;

   // Double use of i and j.
   i = dummy(42);
   j = dummy(42);
   dummy2((long)i * (long)j);
   i = -i;
   dummy2((long)i * (long)j);

   // Single use of i and j.
   i = dummy(42);
   j = dummy(42);
   dummy2((long)i * (long)j);
}

