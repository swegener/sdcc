/* bug-751703.c

   If test_index is char, loses high bit when indexes table 
   workaround is to use [(unsigned int) test_index] 
 */
#include <testfwk.h>

int x = 1;
int y = 2;
int z = 0;

static void
addxy(void)
{
  extern int x, y, z;
  z = x+y;
} 

static void
times10x(void)
{
  unsigned char x;
  
  z = 0;
  for (x=0; x<10; x++)
    {
      extern int x; /* bind to the global x */
      z += x;
    }
}

static void
testExternDeadCode(void)
{
  ASSERT(z == 0);
  addxy();
  ASSERT(z == 3);
  times10x();
  ASSERT(z == 10);
}
