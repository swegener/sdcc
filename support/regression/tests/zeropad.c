/** Zeropad tests.

    storage: idata, xdata, code,
*/
#ifndef STORAGE
#define STORAGE {storage}
#endif

#include <testfwk.h>

typedef unsigned int size_t;
#define offsetof(s,m)   (size_t)&(((s *)0)->m)

#if defined(PORT_HOST) || defined(SDCC_z80) || defined(SDCC_gbz80)
# define idata
# define xdata
# define code
#endif

char array[5] = {'a', 'b', 'c'};

struct w {
  char a;
  int  b;
} STORAGE g[3] = {
  {'x', 1},
  {'y'},
  {'z', 3}
};

struct x {
  short a;
  char  b[10];
};

struct y {
  short a;
  char  b[];
};

struct z {
  char     c;
  struct y s;
};

struct x STORAGE teststruct[5] = {
  { 10, {  1, 2, 3, 4, 5} },
  { 20, { 11 } },
  { 30, {  6, 7, 8} }
};

struct y STORAGE incompletestruct = {
  10, {1, 2, 3, 4, 5}
};

struct z STORAGE nestedstruct = {
  16,
  {20, {6, 7, 8} }
};

void
testZeropad(void)
{
  ASSERT(array[2] == 'c');
  ASSERT(array[4] == 0);

  ASSERT(g[1].a == 'y');
  ASSERT(g[1].b == 0);
  ASSERT(g[2].a == 'z');
  ASSERT(g[2].b == 3);

  ASSERT(teststruct[0].b[1] ==  2);
  ASSERT(teststruct[0].b[5] ==  0);
  ASSERT(teststruct[1].b[0] == 11);
  ASSERT(teststruct[4].b[9] ==  0);

  ASSERT(sizeof(teststruct[2].a) ==  2);
  ASSERT(sizeof(teststruct[1].b) == 10);
  ASSERT(sizeof(teststruct[1])   == 12);
  ASSERT(sizeof(teststruct)      == 60);

  ASSERT(incompletestruct.a    == 10);
  ASSERT(incompletestruct.b[0] ==  1);
  ASSERT(incompletestruct.b[4] ==  5);

  ASSERT(sizeof(incompletestruct) == sizeof(struct y));
  ASSERT(sizeof(incompletestruct) == offsetof(struct y, b));
  ASSERT(sizeof(incompletestruct) == offsetof(struct x, b));

  ASSERT(nestedstruct.c      == 16);
  ASSERT(nestedstruct.s.a    == 20);
  ASSERT(nestedstruct.s.b[2] ==  8);
}
