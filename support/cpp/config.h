#if defined(_MSC_VER)

#include <stdlib.h>
#include "i386/i386.h"
#include "i386/xm-i386.h"
#define alloca(x) Safe_calloc(1,(x))
#define bcopy(s, d, n)  memcpy(d, s, n)
#define bcmp memcmp
#define bzero(p, l) memset(p, 0, l)
#define index strchr
#define rindex strrchr

#else

#include "i386/i386.h"
#include "i386/xm-linux.h"

#ifndef __BORLANDC__
#define alloca(x) Safe_calloc(1,(x))
#else
#include <string.h>
#include <stdlib.h>
#define bcopy(s, d, n)  memcpy(d, s, n)
#define bcmp memcmp
#define bzero(p, l) memset(p, 0, l)
#define index strchr
#define rindex strrchr
#endif

#endif  // _MSC_VER