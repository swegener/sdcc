/*
   bug1757671.c
 */

#include <testfwk.h>

struct c
{
	float r;
};

void clamp(struct c *l)
{
	if(l->r > 240.0f)
		l->r = 240.0f;
	if(l->r < 1.0f)
		l->r = 1.0f;
}

//char cmplong(char x, char y)
char cmplong(long x, long y)
{
	if (x < y)
		return 1;
	return 0;
}

void testBug(void)
{
//	volatile char x = 0xBF;//800000; //-1.0
//	volatile char y = 0x3F;//800000; //+1.0
//	volatile char z = 0x43;//700000; //+240.0
	volatile long x = 0xBF800000; //-1.0
	volatile long y = 0x3F800000; //+1.0
	volatile long z = 0x43700000; //+240.0
	struct c val = { -1.0 };
	clamp(&val);
	ASSERT (val.r == 1.0);
	ASSERT (x <= y);
	ASSERT (cmplong(x, y));
	ASSERT (x < z);
	ASSERT (cmplong(x, z));
}
