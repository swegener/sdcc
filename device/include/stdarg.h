/*-------------------------------------------------------------------------*/
/* stdarg.h - ANSI macros for variable parameter list			   */
/*-------------------------------------------------------------------------*/

#ifndef _STDARG_H
#define _STDARG_H 1

#if defined(SDCC_MODEL_LARGE) || defined(SDCC_MODEL_FLAT24)
typedef	unsigned char _xdata * va_list ;
#elif defined(SDCC_USE_XSTACK)
typedef	unsigned char _pdata * va_list ;
#else
typedef	unsigned char _data *  va_list ;
#endif

#define va_arg(valist,type)   (*((type *)(valist -= sizeof(type))))
#define	va_start(valist,first)	(valist = (va_list)&(first))
#define va_end(valist) (valist = (va_list) 0)

#endif
