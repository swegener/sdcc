/*
   bug-2123.c
 */

#include <testfwk.h>
struct {
  char *p;
  char ct[2];
} arr[4] = {
  {arr[1].ct, "a"},
  {arr->ct, "b"},
  {(*arr).ct, "c"},
  {(&arr[2])->ct, "d"},
};


void testBug(void)
{
  ASSERT (*(arr[0].p) == 'b');
  ASSERT (*(arr[1].p) == 'a');
  ASSERT (*(arr[2].p) == 'a');
  ASSERT (*(arr[3].p) == 'c');
}
