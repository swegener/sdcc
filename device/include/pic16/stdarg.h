/*-----------------------------------------------------------------------------------*/
/* stdarg.h - ANSI macros for variable parameter list				     */
/*-----------------------------------------------------------------------------------*/

#ifndef __STDARG_H
#define __STDARG_H 1

typedef	unsigned char data * va_list ;
#define va_arg(marker,type) *((data type * )(marker += sizeof(type)))
#define	va_start(marker,first) { marker = (va_list) ((char data * )&first); }
#define va_end(marker) marker = (va_list) 0;

#endif
