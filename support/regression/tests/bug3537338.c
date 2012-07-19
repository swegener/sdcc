/*
   bug3537338.c
*/

#include <testfwk.h>

__xdata int globals[3] = {1, 2, 3};
int __xdata * ptr = &globals[1];

int get_signed(signed char index)
{
	return *(ptr+index);
}

int get_unsigned(unsigned char index)
{
	return *(ptr+index);
}

void testBug(void)
{
    ASSERT (get_signed(-1) == globals[0]);
    ASSERT (get_signed(1) == globals[2]);
    ASSERT (get_unsigned(1) == globals[2]);
}
