/** Test framework support functions.
 */
#include <testfwk.h>
#include <stdarg.h>

#ifdef __ds390
#include <tinibios.h> /* main() must see the ISR declarations */
#endif

#if defined(SDCC_mcs51)
/* until changed, isr's must have a prototype in the module containing main */
void T2_isr (void) interrupt 5;
#endif

/** Define this if the port's div or mod functions are broken.
    A slow loop based method will be substituded.
*/
//#define BROKEN_DIV_MOD		1

extern void _putchar(char c);
extern void _initEmu(void);
extern void _exitEmu(void);

#if BROKEN_DIV_MOD
static int
__div(int num, int denom)
{
    int q = 0;
    while (num >= denom) {
        q++;
        num -= denom;
    }
    return q;
}

static int
__mod(int num, int denom)
{
    while (num >= denom) {
        num -= denom;
    }
    return num;
}
#else
#define __div(num, denom) ((num) / (denom))
#define __mod(num, denom) ((num) % (denom))
#endif

static void
_prints(const char *s)
{
  char c;

  while ('\0' != (c = *s)) {
    _putchar(c);
    ++s;
  }
}

static void
_printn(int n)
{
  if (0 == n) {
    _putchar('0');
  }
  else {
    char buf[6];
    char *p = &buf[sizeof(buf) - 1];
    char neg = 0;

    buf[sizeof(buf) - 1] = '\0';

    if (0 > n) {
      n = -n;
      neg = 1;
    }
  
    while (0 != n) {
      *--p = '0' + __mod(n, 10);
      n = __div(n, 10);
    }

    if (neg)
      _putchar('-');

    _prints(p);
  }
}

void
__printf(const char *szFormat, ...)
{
  va_list ap;
  va_start(ap, szFormat);

  while (*szFormat) {
    if (*szFormat == '%') {
      switch (*++szFormat) {
      case 's': {
        char *sz = va_arg(ap, char *);
        _prints(sz);
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

int __numTests = 0;
static int __numFailures = 0;

void
__fail(const char *szMsg, const char *szCond, const char *szFile, int line)
{
  __printf("--- FAIL: \"%s\" on %s at %s:%u\n", szMsg, szCond, szFile, line);
  __numFailures++;
}

int
main(void)
{
  TESTFUNP *cases;
  int numCases = 0;

  _initEmu();

  __printf("--- Running: %s\n", getSuiteName());

  cases = suite();

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
