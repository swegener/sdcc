/*-----------------------------------------------------------------------------------*/
/* stdarg.h - ANSI macros for variable parameter list				     */
/*-----------------------------------------------------------------------------------*/

#ifndef __SDC51_STDARG_H
#define __SDC51_STDARG_H 1

#if defined(SDCC_STACK_TENBIT)

typedef	unsigned char _xdata * va_list ;
#define va_arg(marker,type)   *((type _xdata * )(marker -= sizeof(type)))
#define	va_start(marker,first)	{ marker = (va_list) ((char _xdata * )&first) ; }

#elif defined(SDCC_USE_XSTACK)

typedef	unsigned char _pdata * va_list ;
#define va_arg(marker,type)   *((type _data * )(marker -= sizeof(type)))
#define	va_start(marker,first)	{ marker = (va_list) ((char _pdata * )&first) ; }

#else

typedef	unsigned char _data *  va_list ;
#define va_arg(marker,type)   *((type _data * )(marker -= sizeof(type)))
#define	va_start(marker,first)	{ marker = (va_list) ((char _data * )&first) ; }

#endif

#define va_end(marker) marker = (va_list) 0;

#endif
