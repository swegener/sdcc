
#ifdef TEST1
// No match. This used to result in an internal error when it happened in the initialization of a file-scope object.
int i = _Generic(7, char : 7); /* ERROR */
#endif

void test(void)
{
#ifdef TEST2
  // No match.
  int i = _Generic(7, char : 7); /* ERROR */
#endif
}
