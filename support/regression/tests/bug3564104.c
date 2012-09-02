/*
   bugWHATEVER.c
*/

#include <testfwk.h>

#include <stdint.h>

uintptr_t array[4];

int i;

#if 0
void g(const void *p)
{
	array[i++] = (uintptr_t)(p);
}

void f1(uintptr_t base, const char *message)
{
	for(; *message; message++)
		g((const void *)(base + *message * 8));
}

void f2(uintptr_t base, const char *message)
{
	for(; *message; message++)
	{
		volatile uintptr_t tmp = base + *message * 8;
		g((const void *)(tmp));
	}
}
#endif

void testBug(void)
{
#if 0
	char a[] = {10, 20, 30, 40, 0};

	i = 0;
	f1(0, a);

	ASSERT(array[0] == 10 * 8);
	ASSERT(array[1] == 20 * 8);
	ASSERT(array[2] == 30 * 8);
	ASSERT(array[3] == 40 * 8);

	i = 0;
	f2(0, a);

	ASSERT(array[0] == 10 * 8);
	ASSERT(array[1] == 20 * 8);
	ASSERT(array[2] == 30 * 8);
	ASSERT(array[3] == 40 * 8);
#endif
}

