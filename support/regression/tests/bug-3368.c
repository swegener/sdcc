/* bug-3368.c
   An issue in the handling of array eleemnts passed as struct parameters.
 */
 
#include <testfwk.h>

#include <stdarg.h>

typedef struct{int x,y;}ipoint;
ipoint ipts1[]={{1,2},{3,4}};

#if !defined(__SDCC_mcs51) && !defined(__SDCC_ds390) && !defined(__SDCC_mos6502) && !defined(__SDCC_hc08) && !defined(__SDCC_s08) && (!defined(__SDCC_pdk14) && !defined(__SDCC_pdk15) || defined(__SDCC_STACK_AUTO)) // Todo: enable when struct parameters are supported for these ports!
static int va1(int nargs,ipoint i,...);

void
f1 (void)
{
	va1(4,ipts1[0],ipts1[1]);
}

static int va2(int nargs,...);

void
f2 (void)
{
	va2(4,ipts1[0],ipts1[1]);
}
#endif

void testBug(void)
{
#if !defined(__SDCC_mcs51) && !defined(__SDCC_ds390) && !defined(__SDCC_mos6502) && !defined(__SDCC_hc08) && !defined(__SDCC_s08) && (!defined(__SDCC_pdk14) && !defined(__SDCC_pdk15) || defined(__SDCC_STACK_AUTO)) // Todo: enable when struct parameters are supported for these ports!
	f1();
	f2();
#endif
}

#if !defined(__SDCC_mcs51) && !defined(__SDCC_ds390) && !defined(__SDCC_mos6502) && !defined(__SDCC_hc08) && !defined(__SDCC_s08) && (!defined(__SDCC_pdk14) && !defined(__SDCC_pdk15) || defined(__SDCC_STACK_AUTO)) // Todo: enable when struct parameters are supported for these ports!
static int va1(int nargs,ipoint i1,...)
{
	va_list args;
	ipoint i2;

	ASSERT(i1.x == 1);
	ASSERT(i1.y == 2);

	va_start(args,i1);

	i2 = va_arg(args,ipoint);
	ASSERT(i2.x == 3);
	ASSERT(i2.y == 4);
}

static int va2(int nargs,...)
{
	va_list args;
	ipoint i1, i2;

	va_start(args,nargs);

	i1 = va_arg(args,ipoint);
	ASSERT(i1.x == 1);
	ASSERT(i1.y == 2);

	i2 = va_arg(args,ipoint);
	ASSERT(i2.x == 3);
	ASSERT(i2.y == 4);
}
#endif

