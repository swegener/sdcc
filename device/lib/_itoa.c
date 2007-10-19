/*-------------------------------------------------------------------------
 integer to string conversion

 Written by:   Bela Torok, 1999
               bela.torok@kssg.ch
 usage:

 _uitoa(unsigned int value, char* string, int radix)
 _itoa(int value, char* string, int radix)

 value  ->  Number to be converted
 string ->  Result
 radix  ->  Base of value (e.g.: 2 for binary, 10 for decimal, 16 for hex)
---------------------------------------------------------------------------*/

void _uitoa(unsigned int value, char* string, unsigned char radix)
{
  signed char index = 0, i = 0;

  /* generate the number in reverse order */
  do {
    string[index] = '0' + (value % radix);
    if (string[index] > '9')
        string[index] += 'A' - '9' - 1;
    value /= radix;
    ++index;
  } while (value != 0);

  /* null terminate the string */
  string[index--] = '\0';

  /* reverse the order of digits */
  while (index > i) {
    char tmp = string[i];
    string[i] = string[index];
    string[index] = tmp;
    ++i;
    --index;
  }
}

void _itoa(int value, char* string, unsigned char radix)
{
  if (value < 0 && radix == 10) {
    *string++ = '-';
    value = -value;
  }
  _uitoa(value, string, radix);
}
