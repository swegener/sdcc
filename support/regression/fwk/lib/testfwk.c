/** Test framework support functions.
 */
#include <testfwk.h>
#include <stdarg.h>

/** Define this if the port's div or mod functions are broken.
    A slow loop based method will be substituded.
*/
#define BROKEN_DIV_MOD		1

void _putchar(char c);
void _exitEmu(void);

#if BROKEN_DIV_MOD
int __div(int num, int denom)
{
    int q = 0;
    while (num >= denom) {
        q++;
        num -= denom;
    }
    return q;
}

int __mod(int num, int denom)
{
    while (num >= denom) {
        num -= denom;
    }
    return num;
}
#else
int __div(int num, int denom)
{
    return num/denom;
}

int __mod(int num, int denom)
{
    return num%denom;
}
#endif

static void _printn(int n) 
{
    int rem;

    if (n < 0) {
        _putchar('-');
        n = -n;
    }

    rem = __mod(n, 10);
    if (rem != n) {
        _printn(__div(n, 10));
    }
    _putchar('0' + rem);
}

#ifdef SDCC_mcs51
union
{
    struct
    {
      int  offset;
      char data_space;
    } part;
    char *p;
} generic_p_u;
#endif

void __printf(const char *szFormat, ...) REENTRANT
{
    va_list ap;
    va_start(ap, szFormat);

    while (*szFormat) {
        if (*szFormat == '%') {
            switch (*++szFormat) {
            case 's': {
#ifdef SDCC_mcs51
#warning Workaround bug #436344
                char GENERIC *sz;

                generic_p_u.part.data_space = va_arg (ap, char);
                generic_p_u.part.offset     = va_arg (ap, int);
                sz = generic_p_u.p;
#else
                char GENERIC *sz = va_arg(ap, char GENERIC *);
#endif
                while (*sz) {
                    _putchar(*sz++);
                }
                break;
            }
            case 'u': {
                int i = va_arg(ap, int);
                _printn(i);
                break;
            }
            case '%':
                _putchar('%');
                break;
            default:
                break;
            }
        }
        else {
            _putchar(*szFormat);
        }
        szFormat++;
    }
    va_end(ap);
}

int __numTests;
int __numFailures;

void 
__fail(const char *szMsg, const char *szCond, const char *szFile, int line)
{
    __printf("--- FAIL: \"%s\" on %s at %s:%u\n", szMsg, szCond, szFile, line);
    __numFailures++;
}

int 
main(void)
{
    TESTFUN **cases;
    int numCases = 0;

    __printf("--- Running: %s\n", getSuiteName());

    cases = (TESTFUN **)suite();

    while (*cases) {
        __printf("Running %u\n", numCases);
        (*cases)();
        cases++;
        numCases++;
    }
    
    __printf("--- Summary: %u/%u/%u: %u failed of %u tests in %u cases.\n", 
           __numFailures, __numTests, numCases,
           __numFailures, __numTests, numCases
           );

    _exitEmu();

    return 0;
}
