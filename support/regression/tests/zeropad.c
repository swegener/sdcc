/** Zeropad tests.

    storage: idata, xdata, code,
*/
#ifndef STORAGE
#define STORAGE {storage}
#endif

#if defined __GNUC__
  #define FLEXARRAY (__GNUC__ >= 3)
  //since g fails on GCC 2.95.4 on alpha and I don't know how to detect alpha...
  #define TEST_G    (__GNUC__ >= 3)
#else
  #define FLEXARRAY 1
  #define TEST_G    1
#endif

#include <testfwk.h>

typedef unsigned int size_t;
#define offsetof(s,m)   (size_t)&(((s *)0)->m)

#if defined(PORT_HOST) || defined(SDCC_z80) || defined(SDCC_gbz80)
# define idata
# define xdata
# define code
#endif

const char *string1 = "\x00\x01";
const char string2[] = "\x00\x01";

#ifndef PORT_HOST
#pragma disable_warning 147 //no warning about excess initializers (W_EXCESS_INITIALIZERS)
//array will be truncated but warning will be suppressed
//if truncation is incorrect, other ASSERTs will fail with high probability
char STORAGE trunc[2] = {'a', 'b', 'c'};
#endif

char STORAGE array[5] = {'a', 'b', 'c'};

#if TEST_G
  struct w {
    char a;
    int  b;
  } STORAGE g[3] = {
    {'x', 1},
    {'y'},
    {'z', 3}
  };
#endif

struct x {
  short a;
  char  b[10];
};

struct x STORAGE teststruct[5] = {
  { 10, {  1, 2, 3, 4, 5} },
  { 20, { 11 } },
  { 30, {  6, 7, 8} }
};

#if FLEXARRAY
  struct y {
    short a;
    char  b[];
  };

struct y STORAGE incompletestruct = {
  10, {1, 2, 3, 4, 5}
};
#endif

void
testZeropad(void)
{
  ASSERT(string1[1] == '\x01');
  ASSERT(string2[1] == '\x01');

  ASSERT(array[2] == 'c');
  ASSERT(array[4] == 0);

#if TEST_G
  ASSERT(g[1].a == 'y');
  ASSERT(g[1].b == 0);
  ASSERT(g[2].a == 'z');
  ASSERT(g[2].b == 3);
#endif

  ASSERT(teststruct[0].b[1] ==  2);
  ASSERT(teststruct[0].b[5] ==  0);
  ASSERT(teststruct[1].b[0] == 11);
  ASSERT(teststruct[4].b[9] ==  0);

  ASSERT(sizeof(teststruct[2].a) ==  2);
  ASSERT(sizeof(teststruct[1].b) == 10);
  ASSERT(sizeof(teststruct[1])   == 12);
  ASSERT(sizeof(teststruct)      == 60);

#if FLEXARRAY
  ASSERT(incompletestruct.a    == 10);
  ASSERT(incompletestruct.b[0] ==  1);
  ASSERT(incompletestruct.b[4] ==  5);

  ASSERT(sizeof(incompletestruct) == sizeof(struct y));
  ASSERT(sizeof(incompletestruct) == offsetof(struct y, b));
  ASSERT(sizeof(incompletestruct) == offsetof(struct x, b));
#endif
}
