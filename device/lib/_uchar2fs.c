#include <_float.h>

/* convert unsigned char to float */
float __uchar2fs (unsigned char uc) {
  unsigned long ul=uc;
  return __ulong2fs(ul);
}
