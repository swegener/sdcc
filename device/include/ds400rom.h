// Interface to DS80C400 ROM functions.

#ifndef DS400ROM_H_
#define DS400ROM_H_

extern unsigned char rom_init(void xdata *loMem,
			      void xdata *hiMem) _naked;

// Utility functions.

// A wrapper which calls rom_init allocating all available RAM in CE0
// to the heap.
unsigned char romInit(unsigned char noisy);

// Install an interrupt handler.
void installInterrupt(void (*isrPtr)(void), unsigned char offset);

#endif
