volatile int i;

#ifdef TEST0
void foo(void)
{
  i = 10000 * 10000;	/* WARNING(SDCC) */
  i = 0x4000 * 0x4000;	/* WARNING(SDCC) */
}
#endif

#ifdef TEST1
void foo(void)
{
  i = 1  << 10;
  i = 1  << 16;		/* WARNING(SDCC) */
  i = 1u << 16;		/* WARNING(SDCC) */
  i = 1L << 31;
  i = 1L << 32;		/* WARNING */
}
#endif

