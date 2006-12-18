/*
   bug-1351710.c
*/

#include <testfwk.h>

const char *
const_str(void)
{
return "A"



/* just for testing, actually nop */
#pragma save





"B";
#pragma restore
}

void
testBug(void)
{
  const char *str = const_str();
}
