/*
   test the new C2x keywords
 */

#include <testfwk.h>

#ifdef __SDCC
#pragma std_c2x

alignas(int) int i = alignof(int);
alignas(1) char c;
static_assert(1);
bool b = 0;
bool t = true;
bool f = false;
void *p = nullptr;

#endif

void
testC2Xkey(void)
{
#ifdef __SDCC
  ASSERT(!b);
  ASSERT(t);
  ASSERT(!f);
  ASSERT(!p);
#endif
}

