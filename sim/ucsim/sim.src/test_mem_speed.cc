#include "memcl.h"

int
main(void)
{
  class cl_mem *mem;
  t_addr a;
  t_mem d;

  mem= new cl_mem(MEM_SFR, "egy", 0x10000, 8);
  for (a= 0; a < mem->size; a++)
    {
      t_mem d2= a;
      mem->write(a, &d2);
      d= mem->read(a);
    }
  return(0);
}
