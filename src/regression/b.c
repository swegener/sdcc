
unsigned char success = 0;
unsigned char failures = 0;
unsigned char dummy = 0;

unsigned char uchar0=0;
void done(void)
{
  dummy++;
}

void main(void)
{

  uchar0=1;

  success = failures;
  done();

}
