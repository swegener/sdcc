#ifndef SDCC_VC_HEADER
#define SDCC_VC_HEADER


#undef SDCC_VERSION_HI
#undef SDCC_VERSION_LO
#undef SDCC_VERSION_P
#undef SDCC_VERSION_STR

#ifndef __FUNCTION__
#define __FUNCTION__  __FILE__
#endif

#define DIR_SEPARATOR_STRING "\\"
#define DIR_SEPARATOR_CHAR   '\\'

#define PREFIX                "\\sdcc"
#define EXEC_PREFIX           "\\sdcc"
#define BIN_PREFIX            "\\sdcc\\bin"
#define DATADIR               "\\sdcc"

#define INCLUDE_DIR_SUFFIX    "\\include"
#define LIB_DIR_SUFFIX        "\\lib"

#define BIN2DATA_DIR          "\\.."
#define PREFIX2BIN_DIR        "\\bin"
#define PREFIX2DATA_DIR       ""
#define BINDIR PREFIX		  "\\bin"

/* environment variables */
#define SDCC_DIR_NAME "SDCC_HOME"
#define SDCC_INCLUDE_NAME "SDCC_INCLUDE"
#define SDCC_LIB_NAME "SDCC_LIB"

/* standard libraries */
#define STD_LIB               "libsdcc"
#define STD_INT_LIB           "libint"
#define STD_LONG_LIB          "liblong"
#define STD_FP_LIB            "libfloat"
#define STD_DS390_LIB         "libds390"
#define STD_DS400_LIB	      "libds400"
/* #undef STD_XA51_LIB */

/* #undef HAVE_SYS_SOCKET_H */
#define HAVE_MALLOC_H

#undef HAVE_STRERROR
#define HAVE_VSNPRINTF
#define vsnprintf _vsnprintf
#undef HAVE_SPRINTF
#undef HAVE_VSPRINTF
#undef HAVE_MKSTEMP
#define pclose  _pclose

#define RETSIGTYPE void

#define TYPE_BYTE char
#define TYPE_WORD short
#define TYPE_DWORD int
#define TYPE_UBYTE unsigned TYPE_BYTE
#define TYPE_UWORD unsigned TYPE_WORD
#define TYPE_UDWORD unsigned TYPE_DWORD

/* #undef WORDS_BIGENDIAN */

#undef OPT_DISABLE_MCS51
#undef OPT_DISABLE_GBZ80
#undef OPT_DISABLE_Z80
#undef OPT_DISABLE_AVR
#undef OPT_DISABLE_TININative
#undef OPT_DISABLE_PIC
#undef OPT_DISABLE_XA51

#endif  /* SDCC_VC_HEADER */

/* End of sdcc_vc.h */
