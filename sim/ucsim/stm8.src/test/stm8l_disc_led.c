// Source code under CC0 1.0
#include <stdint.h>

#define PC_ODR	(*(volatile uint8_t *)0x500a)
#define PC_DDR	(*(volatile uint8_t *)0x500c)
#define PC_CR1	(*(volatile uint8_t *)0x500d)

#define PE_ODR	(*(volatile uint8_t *)0x5014)
#define PE_DDR	(*(volatile uint8_t *)0x5016)
#define PE_CR1	(*(volatile uint8_t *)0x5017)

#define CLK_DIVR	(*(volatile uint8_t *)0x50c0)
#define CLK_PCKENR2	(*(volatile uint8_t *)0x50c4)

#define TIM1_CR1	(*(volatile uint8_t *)0x52b0)
#define TIM1_PCNTRH	(*(volatile uint8_t *)0x52bf)
#define TIM1_PCNTRL	(*(volatile uint8_t *)0x52c0)
#define TIM1_PSCRH	(*(volatile uint8_t *)0x52c1)
#define TIM1_PSCRL	(*(volatile uint8_t *)0x52c2)

unsigned int clock(void)
{
	unsigned char h = TIM1_PCNTRH;
	unsigned char l = TIM1_PCNTRL;
	return((unsigned int)(h) << 8 | l);
}

void main(void)
{
	CLK_DIVR = 0x00; // Set the frequency to 16 MHz
	CLK_PCKENR2 |= 0x02; // Enable clock to timer

	// Configure timer
	// 1000 ticks per second
	TIM1_PSCRH = 0x3e;
	TIM1_PSCRL = 0x80;
	// Enable timer
	TIM1_CR1 = 0x01;

	// Configure pins
	PE_DDR = 0x80;
	PE_CR1 = 0x80;

	PC_DDR = 0x80;
	PC_CR1 = 0x80;

	for(;;)
	{
		PE_ODR &= 0x7f;
		if (clock() % 1000 <= 500)
			PE_ODR |= 0x80;
        PC_ODR &= 0x7f;
		if (clock() % 2000 <= 1000)
			PC_ODR |= 0x80;
	}
}
