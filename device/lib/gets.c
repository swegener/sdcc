#include <stdio.h>

char * gets(char *s) {
  char c;
  unsigned int count=0;
  
  while (1) {
    c=getchar();
    switch(c) {
    case '\b': // backspace
      if (count) {
	putchar ('\b');
	putchar (' ');
	putchar ('\b');
	s--;
	count--;
      }
      break;
    case '\n':
    case '\r': // CR or LF
      putchar('\r');
      putchar('\n');
      *s=0;
      return s;
    default:
      *s++=c;
      putchar(c);
      break;
    }
  }
}
