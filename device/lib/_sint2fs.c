#include <float.h>

/* convert signed int to float */
float __sint2fs (signed int si) {
  return __slong2fs(si);
}
