/* Tests wide character conversion functions.
 */
#include <testfwk.h>

#if defined (__SDCC)

#include <uchar.h>
#include <wchar.h>
#include <limits.h>

mbstate_t ps;

#endif

static void
testwchar(void)
{
#if defined (__SDCC)
#if !defined(__SDCC_hc08) && !defined(__SDCC_s08)
	wchar_t w;
	char c[MB_LEN_MAX];

	c[0] = 'C';
	ASSERT(mbrtowc(&w, c, 1, &ps) == 1);
	ASSERT(wcrtomb(c, w, &ps) == 1);
	ASSERT(c[0] == 'C');
#endif
#endif
}

