#include <float.h>

/* convert float to signed long */
signed long __fs2slong (float f) {

  if (!f)
    return 0;

  if (f<0) {
    return -__fs2ulong(-f);
  } else {
    return __fs2ulong(f);
  }
}
