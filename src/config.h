/** @name Global configuration file
 */
#ifndef CONFIG_INCLUDE
#define CONFIG_INCLUDE

#if 0
/** Options specific to this port */
#include "port.h"

/** Set to the name of the C pre-processor.
    Alternatives: sdcpp
 */
#define CPP_NAME "cpp"

/** Set to the option which puts cpp into C++ mode */
#if __GNUC__ > 1 && __GNUC_MINOR__>=95
/* PENDING: Strictly -xc++ */
#define CPP_CPP_LANG_SELECT_OPTION "-xc"
#else
#define CPP_CPP_LANG_SELECT_OPTION "-lang-c++"
#endif
#endif

/** Set to allow '-v' to be used to print out the command being executed */
#define FEATURE_VERBOSE_EXEC	1

#endif
