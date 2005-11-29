/* Check basic behaviour of "flexible array members"
 */
#include <testfwk.h>

struct str1
{
  char c;
  short i[];
};

struct str1 s11 = { 1, {2, 3} };
struct str1 s12 = { 4, {5, 6, 7} }; /* different size */

static void
testFlexibleArray1(void)
{
  /* test sizeof */
  ASSERT(sizeof(s11) == 1);
  /* test allocation size */
#if ! defined(PORT_HOST)
   ASSERT((char *) &s12 - (char *) &s11 == 1 + 4);
#endif
}


/* test initialisation with string */

struct str2
{
  short s;
  char str2[];
};

struct str2 s21 = { 1, "sdcc" };
struct str2 s22 = { 2, "sdcc is great" }; /* different size */

static void
testFlexibleArray2(void)
{
  /* test sizeof */
  ASSERT(sizeof(s21) == 2);
  /* test allocation size */
#if ! defined(PORT_HOST)
   ASSERT((char *) &s22 - (char *) &s21 == 2 + 5);
#endif
}
