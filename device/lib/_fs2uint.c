#include <limits.h>

unsigned long __fs2ulong (float a1);

/* convert float to unsigned int */
unsigned int __fs2uint (float f) {
  unsigned long ul=__fs2ulong(f);
  if (ul>=UINT_MAX) return UINT_MAX;
  return ul;
}

