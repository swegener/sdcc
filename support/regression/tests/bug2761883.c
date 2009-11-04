/*
   bug2761883.c
 */

#include <testfwk.h>

#ifdef SDCC_STACK_AUTO
#define xdata
#endif

volatile char xdata xx1;
volatile int xdata xx2;

extern void func1( char xdata p1, int xdata p2 );

void
testBug(void)
{
	func1( 14, 16 );	// this would pass p2 in data memory
	ASSERT (xx1 == 14);
	ASSERT (xx2 == 16);
}

void func1( char xdata p1, int xdata p2 )
{
	xx1 = p1;
	xx2 = p2;			// while this tried to get it from xdata
}
