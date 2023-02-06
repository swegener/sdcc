volatile static unsigned char *sif;

void
_putchar(unsigned char c)
{
  *sif= 'p';
  *sif= c;
  return;
}

void
_initEmu(void)
{
  sif= (unsigned char *)0xdfff;
}

void
_exitEmu(void)
{
  *sif= 's';
  return;
}
