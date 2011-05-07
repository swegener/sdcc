/* needed by tests/inline.c */

#ifdef SDCC
#pragma std_sdcc99
#endif

extern inline char inlined_function (void)
{
	return 2;
}
