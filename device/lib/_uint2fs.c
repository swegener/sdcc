#include <_float.h>

/* convert unsigned int to float */
float __uchar2fs (unsigned char ui) {
  unsigned long ul=ui;
  return __ulong2fs(ul);
}
