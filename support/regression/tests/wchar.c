/* Tests wide character conversion functions.
 */
#include <testfwk.h>

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
#include <uchar.h>
#include <stdlib.h>
#include <wchar.h>
#include <limits.h>

mbstate_t ps;
#endif

static void
testnorestart(void)
{
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
	wchar_t w;
	char c[MB_LEN_MAX];

	c[0] = 'C';
	ASSERT(mbtowc(&w, c, 1) == 1);
	ASSERT(wctomb(c, w) == 1);
	ASSERT(c[0] == 'C');
#endif
}

static void
testrestart(void)
{
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
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


