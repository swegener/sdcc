/*
   stm8-conventions.c. Test support for calling convetions of other STM8-compilers.
   typearg: unsigned char, unsigned int
   typeret: unsigned char, unsigned int
   convention: , __raisonance, __iar, __cosmic
 */

#include <testfwk.h>

#ifdef __SDCC_stm8
{typeret} f({typearg} i) {convention}
{
	return ++i;
}

{typeret} g({typearg} i, unsigned int j) {convention}
{
	return i + j;
}

{typeret} h({typearg} i, unsigned int j, unsigned long long k) {convention}
{
	return i + j + k;
}

{typeret} f_wrap_sdcc({typearg} i)
{
	return f(i);
}

#if 0 // Bug #3239
{typeret} f_wrap_{convention}({typearg} i) {convention}
{
	return f(i);
}
#endif
#endif

void
testCallingConvention(void)
{
#ifdef __SDCC_stm8
  ASSERT(f(23) == ({typeret})24);
  ASSERT(g(23, 1001u) == ({typeret})1024ul);
  ASSERT(h(23, 1001u, 10000000000ul) == ({typeret})10000001024ull);
  ASSERT(f_wrap_sdcc(23) == ({typeret})24);
#if 0 // Bug #3239
  ASSERT(f_wrap_{convention}(23) == ({typeret})24);
#endif
#endif
}

