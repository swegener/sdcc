#include <float.h>

/* convert unsigned int to float */
float __uint2fs (unsigned int ui) {
  return __ulong2fs(ui);
}
