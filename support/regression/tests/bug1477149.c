/*  bug1477149.c
    multiple definition of local symbol both normal and debug
    second module is fwk/lib/statics.c
    type: long, float
*/

#include <testfwk.h>

static {type} {type}_1 = 2;

static {type} s_get_{type}_1(void)
{
	{type} alfa = {type}_1;
	{type} beta = {type}_1 + alfa;
	{type} gamma = {type}_1 + beta;
	return alfa + beta + gamma;
}

{type} get_{type}_1(void);

void testBug(void)
{
	ASSERT (s_get_{type}_1() == 12);
	ASSERT (get_{type}_1() == 6);
}
