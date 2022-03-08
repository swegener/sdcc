__sfr __at 0x03 sif;

void
_putchar(unsigned char c)
{
  sif= 'p';
  sif= c;
}

void
_initEmu(void)
{
}

void
_exitEmu(void)
{
  sif= 's';
}
