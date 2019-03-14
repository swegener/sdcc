#include "hw.h"

#include "serial.h"
#include "t0.h"
#include "timer.h"
#include "print.h"

#define T0H 0xfc
#define T0L 0x67

unsigned int t0cnt;
__xdata char *simif;

void t0_it(void) __interrupt (1)
{
  TL0= T0L;
  TH0= T0H;
  if (t0cnt)
    t0cnt--;
}


void process(void)
{
  unsigned char c;
  c= serial_receive();
  if ((c >= 'a' && c <= 'z') ||
      (c >= 'A' && c <= 'Z'))
    c^= 0x20;
  print_c(c);
}

void main(void)
{
  char c= 'a';
  beallitas();
  
  t0cnt= T_kezdoertek(1);
  TL0= t0cnt & 0xff;
  TH0= t0cnt >> 8;
  T0idozito1(0);
  T0beallit(t0cnt);
  ET0= 1;
  T0start();
  EA= 1;
  print("\nStart\n");

  t0cnt= 10;
  for (;;)
    {
      if (serial_received())
	process();
      if (!t0cnt)
	{
	  print_c(c);
	  if (++c > 'z')
	    c= 'a';
	  t0cnt= 10;
	}
    }
}
