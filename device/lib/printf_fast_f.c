// Same as printf_fast, but with floating point enabled

#define SDCC_FLOAT_LIB
#include <float.h>

#ifdef FLOAT_ASM_MCS51

#define FLOAT
#define PRINTF_FAST printf_fast_f

#include "printf_fast.c"

#endif
