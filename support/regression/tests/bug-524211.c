/* Problem with casts and warnings.
 */
#include <testfwk.h>

typedef unsigned short UINT16;
typedef unsigned char UINT8;

typedef struct _Class Class;

typedef struct _String
{
  const UINT8 sz[1];
} String;

struct _Class
{
  String *szName;
};

const Class Bar = {
  "java.lang.Object",
};

void foo(void)
{
}
