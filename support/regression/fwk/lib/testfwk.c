/** Test framework support functions.
 */
#include <testfwk.h>
#include <stdarg.h>

//#include <stdio.h>

void _putchar(char c);

static void _printn(int n) {
    // PENDING
    _putchar('0' + n);
}

static void _printf(const char *szFormat, ...)
{
    va_list ap;
    va_start(ap, szFormat);

    while (*szFormat) {
        if (*szFormat == '%') {
            switch (*++szFormat) {
            case 's': {
                const char *sz = va_arg(ap, const char *);
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
    _printf("--- FAIL: \"%s\" on %s at %s:%u\n", szMsg, szCond, szFile, line);
    __numFailures++;
}

int 
main(void)
{
    TESTFUN **cases;
    int numCases = 0;

    _printf("--- Running: %s\n", getSuiteName());

    cases = (TESTFUN **)suite();

    while (*cases) {
        _printf("Running %u\n", numCases);
        (*cases)();
        cases++;
        numCases++;
    }
    
    _printf("--- Summary: %u/%u/%u: %u failed of %u tests in %u cases.\n", 
           __numFailures, __numTests, numCases,
           __numFailures, __numTests, numCases
           );

    return __numFailures;
}
