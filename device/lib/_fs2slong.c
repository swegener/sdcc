#include <limits.h>

unsigned long __fs2ulong (float a1);

/* convert float to signed long */
signed long __fs2slong (float f) {

  if (!f)
    return 0;

  if (f<0) {
    if (f<=LONG_MIN)
      return LONG_MIN;
    return -__fs2ulong(-f);
  } else {
    if (f>=LONG_MAX)
      return LONG_MAX;
    return __fs2ulong(f);
  }
}
