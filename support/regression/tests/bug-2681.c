/** bug-2681.c
    A bug which resulted in LLVM-compiled SDCC crashing.
*/
#include <testfwk.h>

#include <ctype.h>
#include <stdlib.h>


float simplified_atof(const char * s)
{
    float value, fraction;
    signed char iexp;

    for (value=0.0; isdigit(*s); s++)
    {
        value=10.0*value+(*s-'0');
    }

    if (*s == '.')
    {
        s++;
        for (fraction=0.1; isdigit(*s); s++)
        {
            value+=(*s-'0')*fraction;
            fraction*=0.1;
        }
    }

    if (toupper(*s)=='E')
    {
        s++;
        iexp=(signed char)atoi(s);
        {
            while(iexp!=0)
            {
                if(iexp<0)
                {
                    value*=0.1;
                    iexp++;
                }
                else
                {
                    value*=10.0;
                    iexp--;
                }
            }
        }
    }

    return (value);
}

void testBug(void)
{
  ASSERT(simplified_atof("0.0f") == 0.0f);
}

