/*-----------------------------------------------------------------------------------*/
/* stdarg.h - ANSI macros for variable parameter list				     */
/*-----------------------------------------------------------------------------------*/

#ifndef __SDC51_STDARG_H
#define __SDC51_STDARG_H 1

#if defined(__ds390)

typedef	unsigned char * va_list ;
#define va_arg(marker,type) *((type *)(marker -= sizeof(type)))
#define	va_start(marker,first) { marker = &first; }

#elif defined(SDCC_USE_XSTACK)

typedef	unsigned char _pdata * va_list ;
#define va_arg(marker,type) *((type _data *)(marker -= sizeof(type)))
#define	va_start(marker,first) { marker = (va_list)((char _pdata *)&first); }

#else

typedef	unsigned char _data * va_list ;
#define va_arg(marker,type) *((type _data * )(marker -= sizeof(type)))
#define	va_start(marker,first) { marker = (va_list) ((char _data * )&first); }

#endif

#define va_end(marker) marker = (va_list) 0;

#endif
