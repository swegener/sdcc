/*  Test return of struct / union

    type: int, long

 */
#include <testfwk.h>

struct s
{
	{type} a;
	{type} b;
};

union u
{
	unsigned char a;
	{type} b;
};

struct s f(void)
{
	struct s r;
	r.a = 1;
	r.b = 2;
	return(r);
}

union u g(void)
{
	union u r;
	r.a = 1;
	return(r);
}

int g1(void)
{
	struct s r;
	r = f(); // Assignment of returned struct
	return r.a + r.b;
}

int g2(void)
{
	return f().a + f().b; // Access to member of returned struct
}

int h1(void)
{
	union u r;
	r = g(); // Assignment of returned struct
	return r.a + 1;
}

int h2(void)
{
	return g().a + 1; // Access to member of returned struct
}

void testRet (void)
{
	ASSERT (g1() == 3);
	ASSERT (g2() == 3);
	ASSERT (h1() == 2);
	ASSERT (h2() == 2);
}

