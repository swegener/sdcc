#include <float.h>

/* convert signed char to float */
float __schar2fs (signed char sc) {
  return __slong2fs(sc);
}
