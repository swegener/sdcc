#include <common.h>
#include "gen.h"

typedef enum {
    REG_ID_NONE,
    // Z80
    REG_ID_A,
    REG_ID_B,
    REG_ID_C,
    REG_ID_D,
    REG_ID_E,
    REG_ID_H,
    REG_ID_L,
    REG_ID_AF,
    REG_ID_BC,
    REG_ID_DE,
    REG_ID_HL,
    REG_ID_IX,
    REG_ID_IY,
    // TLCS-900H
    REG_ID_XBC,
    REG_ID_XDE,
    // i186
    REG_ID_CL,
    REG_ID_CH,
    REG_ID_CX,
    REG_ID_DL,
    REG_ID_DH,
    REG_ID_DX,
    REG_ID_MAX
} REG_ID;

enum {
    REG_USED = 1,
    REG_USED_HIDDEN = 2
};

enum {
    REG_TYPE_CND = 1,
    REG_TYPE_GPR = 2
} REG_TYPE;

typedef struct regs {
    int size;
    REG_ID id;
    const char *name;
    int used;
    REG_ID hides[3];
} REG;

#define TEST(_d, _a) \
	(_a) ? (void)0 : (failures++, printf("Test %s \"%s\" failed.\n", #_a, _d), _dumpRegs())

#define NUM_OF(_a)	(sizeof(_a)/sizeof(*(_a)))

typedef struct {
    REG *regs;
} IZT_PORT;

IZT_PORT *izt_port;

void izt_init(IZT_PORT *port);
void izt_assignRegisters (eBBlock **ebbs, int count);
void izt_gen(iCode *ic);
