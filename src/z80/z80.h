/** @file z80/z80.h
    Common definitions between the z80 and gbz80 parts.
*/
#include "common.h"
#include "ralloc.h"

typedef enum {
    SUB_Z80,
    SUB_GBZ80
} Z80_SUB_PORT;

typedef struct {
    Z80_SUB_PORT sub;
} Z80_OPTS;

extern Z80_OPTS z80_opts;

#define wassertl(a,s)	(a) ? 0 : \
 		werror (E_INTERNAL_ERROR,__FILE__,__LINE__, s)

#define wassert(a)    wassertl(a,"code generator internal error")

#define IS_GB  (z80_opts.sub == SUB_GBZ80)

