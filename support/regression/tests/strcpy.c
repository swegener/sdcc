/** tests for strcpy
*/
#include <testfwk.h>
#include <string.h>

static void 
teststrcpy(void)
{
  static char empty[] = "";
  static char string[] = "\1\2\0\3";
  char buf[40] = "abcdefghijklmnopqrstuvwxyz";

  char * result = strcpy(buf, empty);
  ASSERT( strlen(buf) == 0);
  ASSERT( result == buf);

  result = strcpy(buf, string);
  ASSERT( result == buf);
  ASSERT( strlen(buf) == 2);
  ASSERT( buf[0] == '\1');
  ASSERT( buf[1] == '\2');
  ASSERT( buf[3] == 'd');
}
