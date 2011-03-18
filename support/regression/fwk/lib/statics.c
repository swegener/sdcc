/* needed by tests/bug1477149.c */

static long x1 = 1;

static long s_get_x1(void)
{
	long alfa = x1;
	long beta = x1 + alfa;
	long gamma = x1 + beta;
	return alfa + beta + gamma;
}

long get_x1(void)
{
	return s_get_x1();
}
