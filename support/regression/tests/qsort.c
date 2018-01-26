/** qsort.c - test sorting

	type: signed int, signed long
*/

#include <testfwk.h>

#include <stdlib.h>
#include <string.h>

#define NUM_ELEM 20

{type} unsorted[NUM_ELEM] = {120, 110, 90, 100, 190, 190, 190, 130, 125, 80, 132, -8, 20, 40, 60, -10, 20, 30, 40, 50};

const {type} sorted[NUM_ELEM] = {-10, -8, 20, 20, 30, 40, 40, 50, 60, 80, 90, 100, 110, 120, 125, 130, 132, 190, 190, 190};

int cmp(const void *lp, const void *rp) __reentrant
{
	{type} l = *((const {type} *)lp);
	{type} r = *((const {type} *)rp);

	if(l < r)
		return(-1);
	else if (l == r)
		return(0);
	else
		return(1);
}

void testSort(void)
{
	qsort(unsorted, NUM_ELEM, sizeof({type}), &cmp);

	ASSERT(!memcmp(unsorted, sorted, sizeof({type}) * NUM_ELEM));
}

