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

  bool ret_true(void)
  {
    return(true);
  }

  bool ret_false(void)
  {
    return(false);
  }

  volatile bool E;

#if (__SDCC_WEIRD_BOOL == 0)
  bool (* const pa[])(void) = {&ret_true, &ret_false};

  struct s
  {
    bool b;
  };

  struct
  {
    bool b : 1;
  } s2;
#endif

void
testBool(void)
{
	volatile unsigned char z = 2;

#if (__SDCC_WEIRD_BOOL == 0)
	const char *BOOL = "George Boole";

	ASSERT(true);
	ASSERT((*(pa[0]))() == true);
	ASSERT((*(pa[1]))() == false);

	s2.b = (z & 2);
	ASSERT(s2.b);
	s2.b = (bool)(z & 2);
	ASSERT(s2.b);
#endif

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
}
