#if defined(_MSC_VER)

#include <stdlib.h>
#include "i386/i386.h" 
#include "i386/xm-i386.h"
#define alloca Safe_calloc
#define bcopy(s, d, n)  memcpy(d, s, n)
#define bcmp memcmp
#define bzero(p, l) memset(p, 0, l)
#define index strchr
#define rindex strrchr

#else

#include "i386/i386.h" 
#include "i386/xm-linux.h"  

#ifndef __BORLANDC__
#define alloca Safe_calloc
#else
#include <string.h>
#include <stdlib.h>
#define bcopy(s, d, n)  memcpy(d, s, n)
#define bcmp memcmp
#define bzero(p, l) memset(p, 0, l)
#define index strchr
#define rindex strrchr
#endif

#endif	// _MSC_VER