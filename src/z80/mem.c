#include "SDCCglobl.h"
#include "SDCCmem.h"
#include "mem.h"

memmap  *xstack= NULL ;	 /* xternal stack data	       */
memmap  *xdata = NULL;	 /* external data			   */
memmap  *idata = NULL;	 /* internal data upto 256     */
memmap  *bit   = NULL;	 /* bit addressable space      */
memmap  *sfr   = NULL;   /* register space              */
memmap  *sfrbit= NULL;   /* sfr bit space               */
memmap  *overlay=NULL;   /* overlay segment             */

void x_initMem(void)
{
}

