/* bool.c
 */

#include <testfwk.h>

#ifdef SDCC
#pragma std_c99
#endif

#include <stdbool.h>
#include <stdint.h>

#if !defined __SDCC_WEIRD_BOOL
  #define __SDCC_WEIRD_BOOL 0
#endif

#if (__SDCC_WEIRD_BOOL < 2)
  bool ret_true(void)
  {
    return(true);
  }

  bool ret_false(void)
  {
    return(false);
  }

  volatile bool E;
#endif

#if (__SDCC_WEIRD_BOOL == 0)
  bool (* const pa[])(void) = {&ret_true, &ret_false};

  struct s
  {
    bool b;
  };
#endif

void
testBool(void)
{
#if (__SDCC_WEIRD_BOOL == 0)
	ASSERT(true);
	ASSERT((*(pa[0]))() == true);
	ASSERT((*(pa[1]))() == false);
#endif

#if (__SDCC_WEIRD_BOOL < 2)
	E = true;
	ASSERT((E ? 1 : 0) == (!(!E)));
	ASSERT((E += 2) == 1);
	ASSERT((--E, --E, E) == E);

	E = false;
	ASSERT((E ? 1 : 0) == (!(!E)));
	ASSERT((E += 2) == 1);
	ASSERT((--E, --E, E) == E);

	E = 0;   ASSERT(!E); // sets E to 0
	E = 1;   ASSERT(E);  // sets E to 1
	E = 4;   ASSERT(E);  // sets E to 1
	E = 0.5; ASSERT(E);  // sets E to 1
	E = false;
	E++;     ASSERT(E);  // sets E to 1
	E = true;
	E++;     ASSERT(E);  // sets E to 1
	E = false;
	E--;     ASSERT(E);  // sets E to 1-E
	E = true;
	E--;     ASSERT(!E); // sets E to 1-E
#endif
}
