/** Bug 868103 tests.

    storage1: near, far, code,
    storage2: near, far,
*/

#ifndef STORAGE1
#define STORAGE1 {storage1}
#endif

#ifndef STORAGE2
#define STORAGE2 {storage2}
#endif

#include <testfwk.h>

#if defined(PORT_HOST) || defined(SDCC_z80) || defined(SDCC_gbz80)
# define near
# define far
# define code
#endif

typedef struct {
  STORAGE1 char * bar[2];
} foo;

STORAGE1 char c = 'x';
STORAGE2 foo f;

void bug868103(void)
{
  f.bar[1] = &c;
  ASSERT(f.bar[1] == &c);
}
