/** Simple printf implementation
    Again a stub - will use the std one later...
*/

#include <stdio.h>

#define NULL 0

/* A hack because I dont understand how va_arg works...
   sdcc pushes right to left with the real sizes, not cast up
   to an int.
   so printf(int, char, long)
   results in push long, push char, push int
   On the z80 the stack grows down, so the things seem to be in
   the correct order.
 */

typedef char * va_list;
#define va_start(list, last)	list = (char *)&last + sizeof(last)
#define va_arg(list, type)	*(type *)list; list += sizeof(type);

typedef void EMIT(char c, void *pData);


static void _printn(unsigned u, unsigned base, char issigned, EMIT *emitter, void *pData)
{
    const char *_hex = "0123456789ABCDEF";
    if (issigned && ((int)u < 0)) {
	(*emitter)('-', pData);
	u = (unsigned)-((int)u);
    }
    if (u >= base)
	_printn(u/base, base, 0, emitter, pData);
    (*emitter)(_hex[u%base], pData);
}

static void _printf(const char *format, EMIT *emitter, void *pData, va_list va)
{
    while (*format) {
	if (*format == '%') {
	    switch (*++format) {
	    case 'c': {
		char c = va_arg(va, char);
		(*emitter)(c, pData);
		break;
	    }
	    case 'u':
		{
		    unsigned u = va_arg(va, unsigned);
		    _printn(u, 10, 0, emitter, pData);
		    break;
		}
	    case 'd':
		{
		    unsigned u = va_arg(va, unsigned);
		    _printn(u, 10, 1, emitter, pData);
		    break;
		}
	    case 'x':
	 	{
		    unsigned u = va_arg(va, unsigned);
		    _printn(u, 16, 0, emitter, pData);
		    break;
		}
	    case 's': 
		{
		    char *s = va_arg(va, char *);
		    while (*s) {
			(*emitter)(*s, pData);
			s++;
		    }
		}
	    }
	}
	else {
	    (*emitter)(*format, pData);
	}
	format++;
    }
}

static void _char_emitter(char c, void *pData)
{
    putchar(c);
}

void printf(const char *format, ...)
{
    va_list va;
    va_start(va, format);

    _printf(format, _char_emitter, NULL, va);
}
