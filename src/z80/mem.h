#ifndef MEM_INCLUDE
#define MEM_INCLUDE

/* memory map prefixes  MOF added the DATA,CODE,XDATA,BIT */
#define  OVERLAY_NAME  	"_OVERLAY"
#define  CODE_NAME     	"_CODE"
#define  DATA_NAME     	"_DATA"
#define  STATIC_NAME   	"GSINIT"
#define GENERIC_NAME   	"GENERIC"
#define ISTACK_NAME    	"_STACK"

#define IN_STACK(x)  (IS_SPEC(x) && (SPEC_OCLS(x) == istack ))
#define IN_FARSPACE(x) FALSE

typedef struct {
} X_MEMMAP;

#endif
