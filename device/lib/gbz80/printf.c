/** Simple printf implementation
    Again a stub - will use the std one later...
*/

#include <stdarg.h>
#include <stdio.h>

/* PENDING */
#define NULL	0

static void _printn(unsigned u, unsigned base, char issigned, void (*emitter)(), void *pData)
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

static void _printf(const char *format, void (*emitter)(), void *pData, va_list va)
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

void putchar(char c);

static void _char_emitter(char c, void *pData)
{
    /* PENDING: Make the compiler happy. */
    pData = 0;

    putchar(c);
}

int printf(const char *format, ...)
{
    va_list va;
    va_start(va, format);

    _printf(format, _char_emitter, NULL, va);
    _printf(format, _char_emitter, NULL, va);

    /* PENDING: What to return? */
    return 0;
}
