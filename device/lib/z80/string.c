/* Dumb strings stub.
   Wanted a quick hack for now - will use the libc version later.
*/
char *strcpy(char *dest, const char *source)
{
    char *ret = dest;
    while (*dest++ = *source++);
    return ret;
}

void *memcpy(void *dest, const void *source, int count)
{
    char *d = dest;
    const char *s = source;
    while (count--)
	*d++ = *s++;

    return dest;
}

int strcmp(const char *s1, const char *s2)
{
    int ret = 0;

    while (!(ret = *s1 - *s2) && *s2)
	++s1, ++s2;

    if (ret < 0)
	return -1;
    else if (ret > 0)
	return 1;
    return 0;
}
