// Interface to DS80C400 ROM functions.

#ifndef DS400ROM_H_
#define DS400ROM_H_

extern unsigned char init_rom(void __xdata *loMem,
			      void __xdata *hiMem) __naked;

extern unsigned long task_gettimemillis_long(void) __naked;

extern unsigned char task_getthreadID(void) __naked;

/** Timer reload value for 14.746 MHz crystal. */
#define RELOAD_14_746 0xfb33

/** Timer reload value for 18.432 MHz crystal. */
#define RELOAD_18_432 0xfa00

/** Timer reload value for 29.491 MHz crystal. */
#define RELOAD_29_491 0xfd99

/** Timer reload value for 36.864 MHz crystal. */
#define RELOAD_36_864 0xfd00

/** Timer reload value for 58.982 MHz crystal. */
#define RELOAD_58_982 0xfecc

/** Timer reload value for 73.728 MHz crystal. */
#define RELOAD_73_728 0xfe80

extern unsigned int task_gettickreload(void);

extern void task_settickreload(unsigned);

#endif
