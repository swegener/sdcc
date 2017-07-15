/*
  bug-2625.c
*/

#include <testfwk.h>

struct s {
  int a,b,c;
  unsigned char array[512-3*sizeof(int)];
};


int check_ptr(unsigned char *p, struct s *sp)
{
  if (p > sp->array + 400) /* Converting the array to its address for the add  */
    return 1;              /* incorrectly used the size of the array to        */
  else                     /* determine the size of the resulting pointer.     */
    return 0;
}


__xdata struct s teststruct;

void testBug (void)
{
  struct s * sp = &teststruct;
  
  ASSERT (check_ptr(&(sp->array[400]),sp) == 0);
  ASSERT (check_ptr(&(sp->array[401]),sp) == 1);
}
