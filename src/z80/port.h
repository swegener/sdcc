/** Set to target the 8051 */
#define PROCESSOR_Z80		1

#define HAS_BITSPACE		0
#define HAS_REGSPACE		0
#define HAS_PAGEDSPACE		0
#define HAS_DIRECTSPACE		0
#define HAS_XDATA		0
#define HAS_OVERLAY		0
#define HAS_IDATA		0
#define HAS_SFRSPACE		0
#define HAS_SFRBITSPACE		0
#define HAS_XSTACK		0

#define HAS_INTERRUPT_TABLE	0

/** Setting this is equivalent to --stack-auto --no-peep --main-return --callee-saves --noregparms --xstack */
#define USE_CLEAN_OPTIONS	1
/** Set if the callee should save _all_ registers */
#define USE_CALLEE_SAVES_ALL	1

