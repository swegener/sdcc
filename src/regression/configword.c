#define __16F873
#include "p16f873.h"
/* configword.c - illustrates how the configuration word can
 * be assigned */

unsigned char success=0;
unsigned char failures=0;
unsigned char dummy=0;

/* copied from 16f877.inc file supplied with gpasm */

#define _CP_ALL          0x0FCF
#define _CP_HALF         0x1FDF
#define _CP_UPPER_256    0x2FEF
#define _CP_OFF          0x3FFF
#define _DEBUG_ON        0x37FF
#define _DEBUG_OFF       0x3FFF
#define _WRT_ENABLE_ON   0x3FFF
#define _WRT_ENABLE_OFF  0x3DFF
#define _CPD_ON          0x3EFF
#define _CPD_OFF         0x3FFF
#define _LVP_ON          0x3FFF
#define _LVP_OFF         0x3F7F
#define _BODEN_ON        0x3FFF
#define _BODEN_OFF       0x3FBF
#define _PWRTE_OFF       0x3FFF
#define _PWRTE_ON        0x3FF7
#define _WDT_ON          0x3FFF
#define _WDT_OFF         0x3FFB
#define _LP_OSC          0x3FFC
#define _XT_OSC          0x3FFD
#define _HS_OSC          0x3FFE
#define _RC_OSC          0x3FFF

typedef unsigned int word;

word at 0x2007  CONFIG = _WDT_OFF & _PWRTE_ON;


/* to do -- write a test that puts the PIC to sleep,
 * and verify that the WDT wakes it up */

void done()
{

  dummy++;

}
void main(void)
{
  dummy = 0;

  success = failures;
  done();
}
