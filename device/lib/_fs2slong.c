#include <_float.h>

/* convert float to signed long */
signed long __fs2slong (float f) {

  if (!f)
    return 0;

  if (f<0) {
    if (f<=SLONG_MIN)
      return SLONG_MIN;
    return -__fs2ulong(-f);
  } else {
    if (f>=SLONG_MAX)
      return SLONG_MAX;
    return __fs2ulong(f);
  }
}
