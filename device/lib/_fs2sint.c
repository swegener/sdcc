#include <_float.h>

/* convert float to signed int */
signed int __fs2sint (float f) {
  signed long sl=__fs2slong(f);
  if (sl>=SINT_MAX)
    return SINT_MAX;
  if (sl<=SINT_MIN) 
    return -SINT_MIN;
  return sl;
}
