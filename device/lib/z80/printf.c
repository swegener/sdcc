/** Simple printf implementation
    Again a stub - will use the std one later...
*/

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


static void _printhex(unsigned u, EMIT *emitter, void *pData)
{
    const char *_hex = "0123456789ABCDEF";
    if (u >= 0x10)
	_printhex(u/0x10, emitter, pData);
    (*emitter)(_hex[u&0x0f], pData);
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
	    case 'd':
		{
		    unsigned u = va_arg(va, unsigned);
		    _printhex(u, emitter, pData);
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
    putchar(c);
}

void printf(const char *format, ...)
{
    va_list va;
    va_start(va, format);

    _printf(format, _char_emitter, NULL, va);
}
