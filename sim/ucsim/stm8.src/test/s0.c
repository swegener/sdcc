#include <stdint.h>
#include <stdio.h>

#define DEVICE DEV_STM8S208

#include "stm8.h"

//#define CLK_DIVR	(*(volatile uint8_t *)0x50c6)
//#define CLK_PCKENR1	(*(volatile uint8_t *)0x50c7)

//#define UART2_SR	(*(volatile uint8_t *)0x5240)
//#define UART2_DR	(*(volatile uint8_t *)0x5241)
//#define UART2_BRR1	(*(volatile uint8_t *)0x5242)
//#define UART2_BRR2	(*(volatile uint8_t *)0x5243)
//#define UART2_CR2	(*(volatile uint8_t *)0x5245)
//#define UART2_CR3	(*(volatile uint8_t *)0x5246)

//#define UART_CR2_TEN (1 << 3)
//#define UART_CR3_STOP2 (1 << 5)
//#define UART_CR3_STOP1 (1 << 4)
//#define UART_SR_TXE (1 << 7)

int putchar(int c)
{
  while(!(USART->sr & USART_SR_TXE));

  USART->dr = c;
  return c;
}

void main(void)
{
  unsigned long i = 0;

  CLK->ckdivr = 0x00; // Set the frequency to 16 MHz
  CLK->pckenr1 = 0xFF; // Enable peripherals

  USART->cr2 = USART_CR2_TEN; // Allow TX and RX
  USART->cr3 &= ~(USART_CR3_STOP1 | USART_CR3_STOP2); // 1 stop bit
  USART->brr2 = 0x03;
  USART->brr1 = 0x68; // 9600 baud
  
  for(;;)
    {
      printf("Hello World!\n");
      for(i = 0; i < 147456; i++); // Sleep
    }
}
