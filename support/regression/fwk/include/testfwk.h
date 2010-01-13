#ifndef __TESTFWK_H
#define __TESTFWK_H   1

extern int __numTests;
extern const int __numCases;

#ifndef NO_VARARGS
void __printf(const char *szFormat, ...);
#define LOG(_a)     __printf _a
#else
#define LOG(_a)     /* hollow log */
#endif

#ifdef SDCC
 #include <sdcc-lib.h>
#else
 #define _AUTOMEM
 #define _STATMEM
#endif

#if defined(PORT_HOST) || defined(SDCC_z80) || defined(SDCC_gbz80)
# define __data
# define __idata
# define __pdata
# define __xdata
# define __code
# define __near
# define __far
# define __at(x)
# define __reentrant
#endif

#if defined(SDCC_hc08)
# define __idata __data
# define __pdata __data
#endif

#if defined(SDCC_pic16)
# define __idata __data
# define __xdata __data
# define __pdata __data
#endif

void __fail (__code const char *szMsg, __code const char *szCond, __code const char *szFile, int line);
void __prints (const char *s);
void __printn (int n);
__code const char *__getSuiteName (void);
void __runSuite (void);

#define ASSERT(_a)  (++__numTests, (_a) ? (void)0 : __fail ("Assertion failed", #_a, __FILE__, __LINE__))
#define ASSERT_FAILED(_a)  (++__numTests, (_a) ? 0 : (__fail ("Assertion failed", #_a, __FILE__, __LINE__), 1))
#define FAIL()      FAILM("Failure")
#define FAILM(_a)   __fail(_a, #_a, __FILE__, __LINE__)

#ifndef NULL
#define NULL  0
#endif

#define UNUSED(_a)  if (_a) { }

#endif //__TESTFWK_H
