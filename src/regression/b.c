
unsigned char success = 0;

unsigned char uchar0=0;
unsigned char uchar1=0;
unsigned char uchar2=0;


void done(void)
{
  success++;
}

void incptr(unsigned char *ucP)
{
  *ucP = *ucP + 1;
}

void inc(unsigned char k)
{
  uchar0 = uchar0 + k;
}

  //  uchar1 = uchar1 + uchar0;
  //  uchar2 = uchar1 + k;

void main(void)
{

  uchar0=1;
  //incptr(&uchar0);
  inc(uchar0);
  if(uchar0 !=2)
    success++;
  done();
}
