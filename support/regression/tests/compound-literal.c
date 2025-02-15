/** Test compound literals (C99)
 */

#include <testfwk.h>
#include <string.h>

char *str1 = "aaa";
char *str2 = (char[]){'b', 'b', 'b', '\0'};

struct s {
  int i;
};

// struct s S1 = (struct s){0x24};

void testCompoundLiterals(void)
{
  const char *str1_local = "AAA";
  const char *str2_local = "BBB";
  struct s s1 = {0x42};
  struct s s2;
  static struct s s3 = {0x84};
  s2 = (struct s){0x21};
  str2_local = (char[]){'C', 'C', 'C', '\0'};

  ASSERT(strcmp(str1, "aaa") == 0);
  ASSERT(strcmp(str2, "bbb") == 0);
  ASSERT(strcmp(str1_local, "AAA") == 0);
  ASSERT(strcmp(str2_local, "CCC") == 0);
  // ASSERT(S1.i == 0x24);
  ASSERT(s1.i == 0x42);
  ASSERT(s2.i == 0x21);
  ASSERT(s3.i == 0x84);
}
