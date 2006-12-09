#ifndef __SDCC_H
#define __SDCC_H

/*
 * From defaults.h
 */
#ifndef GET_ENVIRONMENT
#define GET_ENVIRONMENT(VALUE, NAME) do { (VALUE) = getenv (NAME); } while (0)
#endif

/* Define results of standard character escape sequences.  */
#define TARGET_BELL	007
#define TARGET_BS	010
#define TARGET_TAB	011
#define TARGET_NEWLINE	012
#define TARGET_VT	013
#define TARGET_FF	014
#define TARGET_CR	015
#define TARGET_ESC	033

#define CHAR_TYPE_SIZE 8
#define WCHAR_TYPE_SIZE 32	/* ? maybe ? */

#define SUPPORTS_ONE_ONLY 0

#define TARGET_OBJECT_SUFFIX ".rel"

#ifndef WCHAR_UNSIGNED
#define WCHAR_UNSIGNED 0
#endif

#ifdef _WIN32
#define HAVE_DOS_BASED_FILE_SYSTEM
#endif

/*
 * From hashtab.h
 */
#ifndef GTY
#define GTY(X)
#endif

#endif  /* __SDCC_H */
