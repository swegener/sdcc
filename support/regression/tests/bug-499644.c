/* Floats
 */
#include <testfwk.h>

const float a = 0.0;

#ifndef __SDCC_pdk14 // Lack of memory
float f(void)
{
  return a * 5;
}
#endif

void testBug(void)
{
}
