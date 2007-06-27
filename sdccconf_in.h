/*
 */

#ifndef SDCCCONF_HEADER
#define SDCCCONF_HEADER


#undef SDCC_VERSION_HI
#undef SDCC_VERSION_LO
#undef SDCC_VERSION_P
#undef SDCC_VERSION_STR

#undef DIR_SEPARATOR_STRING
#undef DIR_SEPARATOR_CHAR

#undef PREFIX
#undef EXEC_PREFIX
#undef BINDIR
#undef DATADIR

#undef INCLUDE_DIR_SUFFIX
#undef LIB_DIR_SUFFIX

#undef BIN2DATA_DIR
#undef PREFIX2BIN_DIR
#undef PREFIX2DATA_DIR

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
#undef STD_DS400_LIB
#undef STD_XA51_LIB

#undef HAVE_STRERROR
#undef HAVE_VSNPRINTF
#undef HAVE_SNPRINTF
#undef HAVE_VSPRINTF
#undef HAVE_MKSTEMP

#undef RETSIGTYPE

#undef TYPE_BYTE
#undef TYPE_WORD
#undef TYPE_DWORD
#undef TYPE_UBYTE
#undef TYPE_UWORD
#undef TYPE_UDWORD

/*
 * find out the endianess of host machine
 * in order to be able to make Mac OS X unified binaries
 */
#if __BIG_ENDIAN__ || _BIG_ENDIAN
/* 1) trust the compiler */
# define WORDS_BIGENDIAN 1
#elif __LITTLE_ENDIAN__
/* do nothing */
#elif (defined BYTE_ORDER && defined BIG_ENDIAN && defined LITTLE_ENDIAN && BYTE_ORDER && BIG_ENDIAN && LITTLE_ENDIAN)
/* 2) trust the header files */
# if BYTE_ORDER == BIG_ENDIAN 
#   define WORDS_BIGENDIAN 1
# endif
#else 
/* 3) trust the configure; this actually doesn't work for unified Mac OS X binaries :-( */
# undef BUILD_WORDS_BIGENDIAN
# if (defined BUILD_WORDS_BIGENDIAN && BUILD_WORDS_BIGENDIAN)
#   define WORDS_BIGENDIAN  1
# endif
/* 4) assume that host is a little endian machine */
#endif

#undef OPT_DISABLE_MCS51
#undef OPT_DISABLE_GBZ80
#undef OPT_DISABLE_Z80
#undef OPT_DISABLE_AVR
#undef OPT_DISABLE_DS390
#undef OPT_DISABLE_DS400
#undef OPT_DISABLE_TININative
#undef OPT_DISABLE_PIC
#undef OPT_DISABLE_PIC16
#undef OPT_DISABLE_XA51
#undef OPT_DISABLE_HC08

#undef OPT_ENABLE_LIBGC

#endif

/* End of config.h */
