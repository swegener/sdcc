#include <stdint.h>
#include <stdio.h>
#include <ctype.h>

#include "stm8.h"

volatile unsigned char *sif= (unsigned char *)0x7fff;

int sifchar(int c)
{
  *sif= 'p';
  *sif= c;
  return c;
}

int putchar(int c)
{
  while(!(UART2_SR & UART_SR_TXE));
  UART2_DR = c;
  return c;
}

volatile uint8_t rx_buf[8];
volatile uint8_t first_free= 0;
volatile uint8_t last_used= 0;

void isr_rx(void) __interrupt(21)
{
  volatile uint8_t d;
  *sif='p';*sif='I';
  if (UART2_SR & UART_SR_RXNE)
    {
      uint8_t n;
      d= UART2_DR;
      n= (first_free+1)%8;
      if (n != last_used)
	{
	  rx_buf[first_free]= d;
	  first_free= n;
	}
    }
}

char received()
{
  //return UART2_SR & UART_SR_RXNE;
  return first_free != last_used;
}

char getchar()
{
  uint8_t o;
  while (!received())
    ;
  o= last_used;
  last_used= (last_used+1)%8;
  return rx_buf[o];
}

void prints(char *s)
{
  char i= 0;
  while (s[i])
    {
      putchar(s[i]);
      i++;
    }
}


void main(void)
{
  unsigned long i = 0;

  CLK_DIVR = 0x00; // Set the frequency to 16 MHz
  CLK_PCKENR1 = 0xFF; // Enable peripherals

  UART2_CR2 = UART_CR2_TEN | UART_CR2_REN; // Allow TX and RX
  UART2_CR3 &= ~(UART_CR3_STOP1 | UART_CR3_STOP2); // 1 stop bit
  UART2_BRR2 = 0x03; UART2_BRR1 = 0x68; // 9600 baud

  UART2_CR2|= UART_CR2_RIEN;
  EI;
  
  printf("Hello World!\n");
  for (;;)
    {
      if (received())
	{
	  char c= getchar();
	  *sif= 'x';*sif= c;
	  putchar(toupper(c));
	}
    }
}
