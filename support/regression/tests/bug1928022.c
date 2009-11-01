/*
	bug 1928022
*/

// the following "C"
//
// struct st { char a[1]; };
// char xdata * const Ptr = &(((struct st xdata *) 0x1234) -> a[0]);
//
// will generate an .asm where a label for `Ptr' is generated,
// but no space is allocated, and no initialization is done.
//
// In this piece of regression test,
// the `Fill...' variables are used to work around
// the missing space allocation.
// The cmp() will then reliably bump into those Fill.. locations

#include <testfwk.h>

struct st_a {
	char a[2];
	struct { char c; } s;
};

char xdata * const Ptr_a1 = &(((struct st_a xdata *) 0x1234) -> a[0]);
char xdata * const Ptr_a2 = &(((struct st_a xdata *) 0x1234) -> a[1]);
long const Fill_a = -1;

char xdata * const Ptr_c1 = &(((struct st_a xdata *) 0x1234) -> s.c);
long const Fill_c = -1;

char cmp(void *a, void *b)
{
	return a == b;
}

void
testBug(void)
{
	ASSERT( cmp(Ptr_a1, (char xdata *) 0x1234) );
	ASSERT( cmp(Ptr_a2, (char xdata *) 0x1235) );
	ASSERT( cmp(Ptr_c1, (char xdata *) 0x1236) );
}
