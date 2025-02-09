/* bug-3013.c
   Segfault when trying to pass struct as vararg parameter.
 */
 
#include <testfwk.h>

#include <stdarg.h>

struct some_struct
{
    int i;
};

void f(int n, ...);

void
testBug(void)
{
    struct some_struct x[1] = {{0xa5}};
    f(0, x[0]);
}

void f(int n, ...)
{
    va_list va;
    va_start (va, n);
    struct some_struct s;
    s = va_arg (va, struct some_struct);
    ASSERT(s.i == 0xa5);
}

