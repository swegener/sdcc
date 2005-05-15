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

  //and from bug 1073386
  sprintf( s, "%04X", 0x8765u );
  ASSERT( 0 == strcmp( s, "8765" ) );

  //and from bug 1193299
  sprintf( s, "%3.3s", "abcd" );
  LOG((s));
  ASSERT( 0 == strcmp( s, "abc" ) );
  sprintf( s, "%-3.3s", "abcd" );
  ASSERT( 0 == strcmp( s, "abc" ) );
  sprintf( s, "%3.3s", "ab" );
  ASSERT( 0 == strcmp( s, " ab" ) );
  sprintf( s, "%-3.3s", "ab" );
  ASSERT( 0 == strcmp( s, "ab " ) );

  ASSERT( s[12]==0x12 );
}
