/** @file z80/z80.h
    Common definitions between the z80 and gbz80 parts.
*/
#include "common.h"
#include "ralloc.h"
#include "gen.h"
#include "peep.h"
#include "support.h"

typedef enum
  {
    SUB_Z80,
    SUB_Z180,
    SUB_R2K,
    SUB_GBZ80
  }
Z80_SUB_PORT;

typedef struct
  {
    Z80_SUB_PORT sub;
    int calleeSavesBC;
    int port_mode;
    int port_back;
    int reserveIY;
    int dump_graphs;
    int oldralloc;
  }
Z80_OPTS;

extern Z80_OPTS z80_opts;

#define IS_Z80 (z80_opts.sub == SUB_Z80)
#define IS_Z180 (z80_opts.sub == SUB_Z180)
#define IS_GB  (z80_opts.sub == SUB_GBZ80)
#define IS_R2K  (z80_opts.sub == SUB_R2K)

#define IY_RESERVED (z80_opts.reserveIY)

#define OPTRALLOC_REMAT (IS_Z80 || IS_Z180 || IS_R2K) /* Enable rematerialization in the new allocator for the ports with exact cost function */
#define OPTRALLOC_EXACT_COST (!IS_GB) // Todo: Implement exact cost for gbz80.
#define OPTRALLOC_HL (!IS_GB)
#define OPTRALLOC_A 1
#define OPTRALLOC_IY !IY_RESERVED

enum
  {
    ACCUSE_A = 1,
    ACCUSE_SCRATCH,
    ACCUSE_IY
  };

