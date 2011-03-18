/*  bug1477149.c
    multiple definition of local symbol both normal and debug
    second module is fwk/lib/statics.c
    type: long, float
*/

#include <testfwk.h>

static {type} x1 = 2;

static {type} s_get_x1(void)
{
	{type} alfa = x1;
	{type} beta = x1 + alfa;
	{type} gamma = x1 + beta;
	return alfa + beta + gamma;
}

{type} get_x1(void);

void testBug(void)
{
	ASSERT (s_get_x1() == 12);
	ASSERT (get_x1() == 6);
}
