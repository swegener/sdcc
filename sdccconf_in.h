/*
 */

#ifndef SDCCCONF_HEADER
#define SDCCCONF_HEADER


#undef SDCC_VERSION_HI
#undef SDCC_VERSION_LO
#undef SDCC_VERSION_P
#undef SDCC_VERSION_STR

#undef PREFIX
#undef DATADIR

#undef BIN_DIR_SUFFIX
#undef SEARCH_SUFFIX
#undef INCLUDE_DIR_SUFFIX
#undef LIB_DIR_SUFFIX

// Obsolete:
#undef SDCC_INCLUDE_DIR
#undef SDCC_LIB_DIR

/* environment variables */
#undef SDCC_DIR_NAME
#undef SDCC_INCLUDE_NAME
#undef SDCC_LIB_NAME

/* standard libraries */
#undef STD_LIB
#undef STD_INT_LIB
#undef STD_LONG_LIB
#undef STD_FP_LIB
#undef STD_DS390_LIB
#undef STD_XA51_LIB

#undef HAVE_SYS_SOCKET_H

#undef HAVE_STRERROR
#undef HAVE_VSNPRINTF
#undef HAVE_SNPRINTF
#undef HAVE_VSPRINTF
#undef HAVE_MKSTEMP

#undef RETSIGTYPE

#undef TYPE_BYTE
#undef TYPE_WORD
#undef TYPE_DWORD
#define TYPE_UBYTE unsigned TYPE_BYTE
#define TYPE_UWORD unsigned TYPE_WORD
#define TYPE_UDWORD unsigned TYPE_DWORD

#undef WORDS_BIGENDIAN

#undef OPT_DISABLE_MCS51
#undef OPT_DISABLE_GBZ80
#undef OPT_DISABLE_Z80
#undef OPT_DISABLE_AVR
#undef OPT_DISABLE_DS390
#undef OPT_DISABLE_TININative
#undef OPT_DISABLE_PIC
#undef OPT_DISABLE_XA51

#undef OPT_ENABLE_LIBGC

#endif

/* End of config.h */
