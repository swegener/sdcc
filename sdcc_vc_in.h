#ifndef SDCC_VC_HEADER
#define SDCC_VC_HEADER


#undef SDCC_VERSION_HI
#undef SDCC_VERSION_LO
#undef SDCC_VERSION_P
#undef SDCC_VERSION_STR

#ifndef __FUNCTION__
#define __FUNCTION__  __FILE__
#endif

#undef PREFIX
#undef DATADIR
#undef SRCDIR

#define PREFIX                "\\sdcc"

//#define STANDARD_INCLUDE_DIR  "\\sdcc\\local\\share\\include"
#define SDCC_INCLUDE_DIR      "\\sdcc\\include"
#define SDCC_LIB_DIR          "\\sdcc\\lib"

#define STD_LIB               "libsdcc"
#define STD_INT_LIB           "libint"
#define STD_LONG_LIB          "liblong"
#define STD_FP_LIB            "libfloat"
#define STD_DS390_LIB         "libds390"

// #undef HAVE_SYS_SOCKET_H
// #undef HAVE_SYS_ISA_DEFS_H	
// #undef HAVE_ENDIAN_H

#undef HAVE_STRERROR

#undef OPT_DISABLE_MCS51
#undef OPT_DISABLE_GBZ80
#undef OPT_DISABLE_Z80
#undef OPT_DISABLE_AVR
#undef OPT_DISABLE_TININative
#undef OPT_DISABLE_PIC
#undef OPT_DISABLE_XA51

#endif  /* SDCC_VC_HEADER */

/* End of sdcc_vc.h */
