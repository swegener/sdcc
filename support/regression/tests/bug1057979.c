/*
   bug1057979.c
*/

#include <testfwk.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

char s[12+1];

void
test_sprintf(void)
{
  s[12] = 0x12;

  sprintf( s, "%d", 99 );
  ASSERT( 0 == strcmp( s, "99" ) );
  sprintf( s, "%d", 100 );
  ASSERT( 0 == strcmp( s, "100" ) );
  sprintf( s, "%d", 2004 );
  ASSERT( 0 == strcmp( s, "2004" ) );
  sprintf( s, "%ld", 2147483647L );
  ASSERT( 0 == strcmp( s, "2147483647" ) );

  ASSERT( s[12]==0x12 );
}
