/*
   bug1057979.c
*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <testfwk.h>

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
#ifdef SDCC_pic16
  //pic16 printf doesn't support flags, width and precision specifiers
  sprintf( s, "%04X", 0x8765u );
  ASSERT( 0 == strcmp( s, "04X" ) );

  //and from bug 1193299
  sprintf( s, "%3.3s", "abcd" );
  ASSERT( 0 == strcmp( s, "3.3s" ) );
  sprintf( s, "%-3.3s", "abcd" );
  ASSERT( 0 == strcmp( s, "-3.3s" ) );
  sprintf( s, "%3.3s", "ab" );
  ASSERT( 0 == strcmp( s, "3.3s" ) );
  sprintf( s, "%-3.3s", "ab" );
  ASSERT( 0 == strcmp( s, "-3.3s" ) );
#else
  sprintf( s, "%04X", 0x8765u );
  ASSERT( 0 == strcmp( s, "8765" ) );

  //and from bug 1193299
  sprintf( s, "%3.3s", "abcd" );
  ASSERT( 0 == strcmp( s, "abc" ) );
  sprintf( s, "%-3.3s", "abcd" );
  ASSERT( 0 == strcmp( s, "abc" ) );
  sprintf( s, "%3.3s", "ab" );
  ASSERT( 0 == strcmp( s, " ab" ) );
  sprintf( s, "%-3.3s", "ab" );
  ASSERT( 0 == strcmp( s, "ab " ) );
#endif

#if defined(SDCC_ds390) || defined(PORT_HOST)
  //and from bug 1358192
  sprintf( s, "%f", 10.1 );
  LOG((s));
  ASSERT( 0 == strcmp( s, "10.100000" ) );

  //and from bug 1388703
  sprintf( s, "%4.1f", 1.36 );
  LOG((s));
  ASSERT( 0 == strcmp( s, " 1.4" ) );
  sprintf( s, "%4.1f", -1.37 );
  LOG((s));
  ASSERT( 0 == strcmp( s, "-1.4" ) );
#endif

  ASSERT( s[12]==0x12 );
}
