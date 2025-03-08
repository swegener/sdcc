/** Test the _Countof operator (C2y) */

#include <testfwk.h>

#ifdef __SDCC
_Pragma("std_c2y")
#endif

void testCountof(void)
{
#ifdef __SDCC
  char arr1[3];
  int arr2[2];
  char arr3[2][3];
  char str1[] = "abc";

  ASSERT(_Countof(arr1) == 3);
  ASSERT(_Countof(arr2) == 2);
  ASSERT(_Countof(str1) == 4);
  ASSERT(_Countof(arr3) == 2);
#endif
}
