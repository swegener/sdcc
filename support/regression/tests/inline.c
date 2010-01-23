/** inline tests
*/

#include <testfwk.h>

#ifdef SDCC
#pragma std_sdcc99
#endif

/*--------------
    bug 1717305
*/

static inline int
f (const int a)
{
	return (a + 3);
}

int g (int b)
{
	return (f (b));
}

void
bug_1717305 (void)
{
  int x = 0;
  ASSERT (g (x) == 3);
}

/*--------------
    bug 1767885
*/

static inline int
f1 (int x)
{
  int b = 6;
  return x + b;
}

static inline int
f2 (int x)
{
  int y = f1 (x);
  return y;
}

static inline int
f3 (int x)
{
  int y;
  y = f1 (x);
  return y;
}

int gi = 6;

void
bug_1767885 (void)
{
  gi = f2 (gi);
  ASSERT (gi == 12);

  gi = f3 (gi);
  ASSERT (gi == 18);
}

/*--------------
    bug 1864577
*/
typedef unsigned char uint8_t;

typedef uint8_t error_t;

enum __nesc_unnamed4247
  {
    SUCCESS = 0, 
    FAIL = 1, 
    ESIZE = 2, 
    ECANCEL = 3, 
    EOFF = 4, 
    EBUSY = 5, 
    EINVAL = 6, 
    ERETRY = 7, 
    ERESERVE = 8, 
    EALREADY = 9
  };

static inline error_t
PlatformP__LedsInit__default__init (void)
{
  return SUCCESS;
}

static error_t
PlatformP__LedsInit__init (void);

static inline error_t
PlatformP__LedsInit__init (void)
{
  unsigned char result;

  result = PlatformP__LedsInit__default__init ();

  return result;
}

void
bug_1864577 (void)
{
  ASSERT (PlatformP__LedsInit__init () == SUCCESS);
}

/*--------------*/

void
testInline (void)
{
  bug_1717305 ();
  bug_1767885 ();
  bug_1864577 ();
}
