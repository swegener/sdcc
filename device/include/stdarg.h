/*-----------------------------------------------------------------------------------*/
/* stdarg.h - ANSI macros for variable parameter list                                */
/*-----------------------------------------------------------------------------------*/

#ifndef __SDC51_STDARG_H
#define __SDC51_STDARG_H 1

#if defined(__z80) || defined(__gbz80) || defined(__hc08)

typedef unsigned char * va_list;
#define va_start(marker, last)  { marker = (unsigned char *)&last + sizeof(last); }
#define va_arg(marker, type)    *((type *)((marker += sizeof(type)) - sizeof(type)))

#elif defined(__ds390) || defined(__ds400)

typedef unsigned char * va_list;
#define va_start(marker, first) { marker = &first; }
#define va_arg(marker, type)    *((type *)(marker -= sizeof(type)))

#elif defined(SDCC_USE_XSTACK)

typedef unsigned char __pdata * va_list;
#define va_start(marker, first) { marker = (va_list)((char __pdata *)&first); }
#define va_arg(marker, type)    *((type __pdata *)(marker -= sizeof(type)))

#else

typedef unsigned char __data * va_list;
#define va_start(marker, first) { marker = (va_list) ((char __data * )&first); }
#define va_arg(marker, type)    *((type __data * )(marker -= sizeof(type)))

#endif

#define va_end(marker) marker = (va_list) 0;

#endif
