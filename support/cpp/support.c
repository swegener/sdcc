/* Support functions for mingw32 and probably Borland C
 */
#include <string.h>

#ifdef __MINGW32__
void bzero(void *s, size_t n)
{
    memset(s, 0, n);
}

void bcopy(const void *src, void *dest, size_t n)
{
    memcpy(dest, src, n);
}

int bcmp(const void *s1, const void *s2, size_t n)
{
    return memcmp(s1, s2, n);
}

char *index(const char *s, int c)
{
    return strchr(s, c);
}

char *rindex(const char *s, int c)
{
    return strrchr(s, c);
}
#endif
