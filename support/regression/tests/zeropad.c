/** Zeropad tests.

    storage: idata, xdata, code,
*/
#include <testfwk.h>

#if defined(PORT_HOST) || defined(SDCC_z80) || defined(SDCC_gbz80)
# define idata
# define xdata
# define code
#endif

struct x {
  short a;
  char  b[10];
};

struct x {storage} teststruct[6] = {
  { 10, {  1, 2, 3, 4, 5} },
  { 20, { 11 } },
  { 30, {  6, 7, 8} }
};

void
testZeropad(void)
{
  ASSERT(teststruct[0].b[1] ==  2);
  ASSERT(teststruct[0].b[5] ==  0);
  ASSERT(teststruct[1].b[0] == 11);

  ASSERT(sizeof(teststruct[2].a) ==  2);
  ASSERT(sizeof(teststruct[1].b) == 10);
  ASSERT(sizeof(teststruct[1])   == 12);
  ASSERT(sizeof(teststruct)      == 72);
}
