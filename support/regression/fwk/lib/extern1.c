/* needed by tests/inline.c */

#ifdef __SDCC
#pragma std_sdcc99
#endif

extern char inlined_function (void)
{
	return 2;
}

/* needed by tests/test-p99-conformance.c */
void
has_undefined_symbol2(void) {
  /* empty */
}

