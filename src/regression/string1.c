unsigned char success=0;
unsigned char failures=0;
unsigned char dummy=0;
//unsigned bit bit1;

typedef unsigned char byte;
data at 0x06 unsigned char  PORTB;

byte uchar0;
const byte arr[] = { 1,2,8,9,0 };

bit at 0x30 B1;

void done()
{

  dummy++;

}

void  isr(void) interrupt 1 using 1
{

  if(arr[3]!=9)
    failures++;
  PORTB = 7;
  B1=1;
}

void lcd1(const unsigned char *str)
{
  const char *p = "hellow world!\r\n";

  if(!str)
    failures++;

 if(str && str[2] != 'r')
    failures++;

  if(arr[3]!=9)
    failures++;

  while (1) {
    if (0 == *p) break;
    PORTB = *p;
    p++;
  }

}

void main(void)
{
  dummy = 0;

  lcd1("str");
  B1=0;
  success = failures;
  done();
}
