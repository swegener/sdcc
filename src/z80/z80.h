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
    int max_allocs_per_node;
    int dump_graphs;
  }
Z80_OPTS;

extern Z80_OPTS z80_opts;

#define IS_GB  (z80_opts.sub == SUB_GBZ80)
#define IS_Z80 (z80_opts.sub == SUB_Z80)
#define IY_RESERVED (z80_opts.reserveIY)

enum
  {
    ACCUSE_A = 1,
    ACCUSE_SCRATCH,
    ACCUSE_IY
  };
