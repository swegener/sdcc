#include "SDCCglue.h"
#include "glue.h"

extern int maxRegBank ;

/* Not yet ported */
#if 0
void x_glue(FILE *asmFile)
{
    assert(asmFile);
}

void x_insertMainHook(FILE *asmFile)
{
    assert(asmFile);

    if (mainf && mainf->fbody) {
	fprintf (asmFile,"__sdcc_gsinit_startup:\n");
    }

}
#endif
