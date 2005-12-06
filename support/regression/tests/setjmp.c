/** setjmp/longjmp tests.
*/
#include <testfwk.h>
#include <setjmp.h>

unsigned int global_int = 0;
unsigned int *gpInt;

#if defined(SDCC_mcs51)
#include <8052.h>

void T2_isr (void) interrupt 5 //no using
{
  //do not clear flag TF2 so it keeps interrupting !
  (*gpInt)++;
}
#endif

#if defined(SDCC_mcs51) || defined(PORT_HOST)

void try_fun(jmp_buf catch, int except)
{
  longjmp(catch, except);
}

#endif

void
testJmp(void)
{
#if defined(SDCC_mcs51) || defined(PORT_HOST)

  jmp_buf catch;
  int exception;

#if defined(SDCC_mcs51)
  gpInt = &global_int;
  //enable the interrupt and set it's flag to generate some heavy stack usage
  ET2 = 1;
  EA = 1;
  TF2 = 1;
#endif

  exception = setjmp(catch);
  if (exception == 0)
  {
  	try_fun(catch, 1);
  	//should not get here!
  	ASSERT(0);
  }
  ASSERT(exception == 1);

#endif
}
