#ifndef STM8_HEADER
#define STM8_HEADER

#define CLK_DIVR	(*(volatile uint8_t *)0x50c6)
#define CLK_PCKENR1	(*(volatile uint8_t *)0x50c7)


#define UART2_SR	(*(volatile uint8_t *)0x5240)
#define UART2_DR	(*(volatile uint8_t *)0x5241)
#define UART2_BRR1	(*(volatile uint8_t *)0x5242)
#define UART2_BRR2	(*(volatile uint8_t *)0x5243)
#define UART2_CR2	(*(volatile uint8_t *)0x5245)
#define UART2_CR3	(*(volatile uint8_t *)0x5246)

#define UART_CR2_TEN (1 << 3)
#define UART_CR2_REN (1 << 2)
#define UART_CR2_RIEN (1 << 5)
#define UART_CR3_STOP2 (1 << 5)
#define UART_CR3_STOP1 (1 << 4)
#define UART_SR_TXE (1 << 7)
#define UART_SR_RXNE (1 << 5)

#define EI __asm__("rim")
#define DI __asm__("sim")

#endif
