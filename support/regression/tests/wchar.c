/* Tests wide character conversion functions.
 */
#include <testfwk.h>

#include <stdlib.h>
#include <limits.h>

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199409L
#include <wchar.h>
#endif
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
#include <uchar.h>
#endif

static void
testwcharnorestart(void)
{
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199409L && !defined(__SDCC_mcs51)
	wchar_t w;
	char c[MB_LEN_MAX];

	c[0] = 'C';
	ASSERT(mbtowc(&w, c, 1) == 1);
	ASSERT(wctomb(c, w) == 1);
	ASSERT(c[0] == 'C');
#endif
}

static void
testwcharrestart(void)
{
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L && !defined(__SDCC_mcs51)
#if !defined(__SDCC_hc08) && !defined(__SDCC_s08)
	static mbstate_t ps;
	wchar_t w;
	char c[MB_LEN_MAX];

	c[0] = 'C';
	ASSERT(mbrtowc(&w, c, 1, &ps) == 1);
	ASSERT(wcrtomb(c, w, &ps) == 1);
	ASSERT(c[0] == 'C');
#endif
#endif
}

static void
testchar16restart(void)
{
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L && !defined(__SDCC_mcs51)
#if !defined(__SDCC_hc08) && !defined(__SDCC_s08)
	static mbstate_t ps;
	char16_t c16[2];
	char c[MB_LEN_MAX];

	c[0] = 'C';
	ASSERT(mbrtoc16(c16, c, 1, &ps) == 1);
	ASSERT(c16rtomb(c, c16[0], &ps) == 1);
	ASSERT(c[0] == 'C');
#endif
#endif
}

