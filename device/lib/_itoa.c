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

#define NUMBER_OF_DIGITS 16

void _uitoa(unsigned int value, char* string, unsigned char radix)
{
unsigned char index, i;
/* char buffer[NUMBER_OF_DIGITS]; */ /* space for NUMBER_OF_DIGITS + '\0' */

  index = NUMBER_OF_DIGITS;
  i = 0;

  do {
    string[--index] = '0' + (value % radix);
    if ( string[index] > '9') string[index] += 'A' - '9' - 1;
    value /= radix;
  } while (value != 0);

  do {
    string[i++] = string[index++];
  } while ( index < NUMBER_OF_DIGITS );

  string[i] = 0; /* string terminator */
}

void _itoa(int value, char* string, unsigned char radix)
{
  if (value < 0 && radix == 10) {
    *string++ = '-';
    _uitoa(-value, string, radix);
  }
  else {
    _uitoa(value, string, radix);
  }
}

