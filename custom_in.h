/* custom.h -- manual additions to #include into sdccconf.h */

/*
 * find out the endianess of host machine
 */
#if defined __APPLE__ && (defined __BIG_ENDIAN__ || defined _BIG_ENDIAN)
/* 1) trust the compiler
 * in order to be able to make Mac OS X unified binaries */
# define WORDS_BIGENDIAN 1
#elif defined __APPLE__ && (defined __LITTLE_ENDIAN__ || defined _LITTLE_ENDIAN)
/* do nothing */
#elif defined __sun && defined HAVE_SYS_ISA_DEFS_H
/* Solaris defines endianness in <sys/isa_defs.h> */
# include <sys/isa_defs.h>
# ifdef _BIG_ENDIAN
#   define WORDS_BIGENDIAN 1
# endif
#else
# ifdef HAVE_ENDIAN_H
#   include <endian.h>
# elif defined HAVE_SYS_ENDIAN_H
#   include <sys/endian.h>
# elif defined HAVE_MACHINE_ENDIAN_H
#   include <machine/endian.h>
# endif
# if (defined BYTE_ORDER && defined BIG_ENDIAN && defined LITTLE_ENDIAN && BYTE_ORDER && BIG_ENDIAN && LITTLE_ENDIAN)
/* 2) trust the header files */
#   if BYTE_ORDER == BIG_ENDIAN
#     define WORDS_BIGENDIAN 1
#   endif
# else
/* 3) trust the configure; this actually doesn't work for unified Mac OS X binaries :-( */
#   if (defined BUILD_WORDS_BIGENDIAN && BUILD_WORDS_BIGENDIAN)
#     define WORDS_BIGENDIAN  1
#   endif
/* 4) assume that host is a little endian machine */
# endif
#endif
