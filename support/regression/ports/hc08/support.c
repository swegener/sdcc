
void
_putchar(unsigned char c)
{
  c;
  _asm
    .db 0x9e, 0xed
  _endasm;
}

void
_exitEmu(void)
{
  _asm
    .db 0x9e, 0xec
  _endasm;
}
