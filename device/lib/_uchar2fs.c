#include <float.h>

/* convert unsigned char to float */
float __uchar2fs (unsigned char uc) {
  return __ulong2fs(uc);
}
