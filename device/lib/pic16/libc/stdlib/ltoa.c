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

#if _DEBUG
extern void io_long(unsigned long);
extern void io_str(char *);
#endif


void ultoa(unsigned long value, __data unsigned char* str, unsigned char radix)
{
  unsigned int index;
  unsigned char ch;
  unsigned char buffer[NUMBER_OF_DIGITS];  /* space for NUMBER_OF_DIGITS + '\0' */

    index = NUMBER_OF_DIGITS;
  
    do {
      ch = '0' + (value % radix);
      if ( ch > '9') ch += 'a' - '9' - 1;

#if _DEBUG
      io_str( "ultoa: " );
      io_long( value );
      io_long( (unsigned long) ch );
#endif

      buffer[ --index ] = ch;
      value /= radix;
    } while (value != 0);

    do {
      *str++ = buffer[index++];
    } while ( index < NUMBER_OF_DIGITS );

    *str = 0;  /* string terminator */
}

void ltoa(long value, __data unsigned char* str, unsigned char radix)
{
#if _DEBUG
  io_str( "ltoa: " );
  io_long( (unsigned long)value );
#endif

  if (value < 0 && radix == 10) {
    *str++ = '-';
    value = -value;
  }



  ultoa((unsigned long)value, str, radix);
}
