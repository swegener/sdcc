#include "i386/i386.h" 
#include "i386/xm-linux.h"  

#ifndef __BORLANDC__
#define alloca malloc
#else
#include <string.h>
#include <stdlib.h>
#define bcopy(s, d, n)  memcpy(d, s, n)
#define bcmp memcmp
#define bzero(p, l) memset(p, 0, l)
#define index strchr
#define rindex strrchr
#endif


