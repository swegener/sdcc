/*-------------------------------------------------------------------------
 integer to string conversion

 Written by:   Bela Torok, 1999
               bela.torok@kssg.ch
 usage:

 _ultoa(unsigned long value, char* string, int radix)
 _ltoa(long value, char* string, int radix)

 value  ->  Number to be converted
 string ->  Result
 radix  ->  Base of value (e.g.: 2 for binary, 10 for decimal, 16 for hex)
---------------------------------------------------------------------------*/

#define NUMBER_OF_DIGITS 32

void ultoa(unsigned long value, data char* string, unsigned char radix)
{
unsigned char index;
char buffer[NUMBER_OF_DIGITS];  /* space for NUMBER_OF_DIGITS + '\0' */

  index = NUMBER_OF_DIGITS;

  do {
    buffer[--index] = '0' + (value % radix);
    if ( buffer[index] > '9') buffer[index] += 'A' - '9' - 1;
    value /= radix;
  } while (value != 0);

  do {
    *string++ = buffer[index++];
  } while ( index < NUMBER_OF_DIGITS );

  *string = 0;  /* string terminator */
}

void ltoa(long value, data char* string, unsigned char radix)
{
  if (value < 0 && radix == 10) {
    *string++ = '-';
    value = -value;
  }

  ultoa(value, string, radix);
}

