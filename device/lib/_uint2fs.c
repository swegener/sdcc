float __ulong2fs (unsigned long a );

/* convert unsigned int to float */
float __uint2fs (unsigned int ui) {
  unsigned long ul=ui;
  return __ulong2fs(ul);
}
