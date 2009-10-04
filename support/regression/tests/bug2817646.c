/* bug2817646.c
 * Structure order error
 */

#include <testfwk.h>

typedef struct dlnode_good {
    struct dlnode_good xdata * nxt;
    struct dlnode_good xdata * prv;
} dlnode_good;

typedef dlnode_good xdata * dlist_good;

xdata dlnode_good good = { &good, NULL };
xdata dlist_good gl = &good;

dlnode_good xdata *
f0( const dlist_good * obj )
{
    return (*obj) ? (*obj)->nxt : 0;
}

dlnode_good xdata *
f1( xdata dlist_good * obj )
{
    return (*obj) ? (*obj)->nxt : 0;
}

dlnode_good xdata *
f2( dlnode_good xdata * const * obj )
{
    return (*obj) ? (*obj)->nxt : 0;
}

dlnode_good xdata *
f3( dlnode_good xdata * xdata * obj )
{
    return (*obj) ? (*obj)->nxt : 0;
}

typedef struct dlnode_bad {
    struct dlnode_bad xdata * prv;
    struct dlnode_bad xdata * nxt;
} dlnode_bad;

typedef dlnode_bad xdata * dlist_bad;

xdata dlnode_bad bad = { NULL, &bad };
xdata dlist_bad bl = &bad;

dlnode_bad xdata *
f4( const dlist_bad * obj )
{
    return (*obj) ? (*obj)->nxt : 0;
}

dlnode_bad xdata *
f5( xdata dlist_bad * obj )
{
    return (*obj) ? (*obj)->nxt : 0;
}

dlnode_bad xdata *
f6( dlnode_bad xdata * const * obj )
{
    return (*obj) ? (*obj)->nxt : 0;
}

dlnode_bad xdata *
f7( dlnode_bad xdata * xdata * obj )
{
    return (*obj) ? (*obj)->nxt : 0;
}

void
testBug(void)
{
	ASSERT (f0(&gl) == &good);
	ASSERT (f1(&gl) == &good);
	ASSERT (f2(&gl) == &good);
	ASSERT (f3(&gl) == &good);

	ASSERT (f4(&bl) == &bad);
	ASSERT (f5(&bl) == &bad);
	ASSERT (f6(&bl) == &bad);
	ASSERT (f7(&bl) == &bad);
}
