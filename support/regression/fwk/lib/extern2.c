/* needed by tests/inline.c */

#ifdef SDCC
#pragma std_sdcc99
#endif

extern char inlined_function (void);

extern char (*inlined_function_pointer) (void) = &inlined_function;
