/** Test the _Lengthof operator (C2y) */

#include <testfwk.h>

#ifdef __SDCC
_Pragma("std_c2y")
#endif

void testLengthof(void)
{
#ifdef __SDCC
  char arr1[3];
  int arr2[2];
  char arr3[2][3];
  char str1[] = "abc";

  ASSERT(_Lengthof(arr1) == 3);
  ASSERT(_Lengthof(arr2) == 2);
  ASSERT(_Lengthof(str1) == 4);
  ASSERT(_Lengthof(arr3) == 2);
#endif
}
