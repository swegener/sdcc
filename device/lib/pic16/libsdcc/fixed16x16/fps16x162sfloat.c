
float __fps16x162sfloat(__fixed16x16 fixd)
{
  union {
    __fixed16x16 fix;
    unsigned long value;
  } u;
  float tmp=0, exp=2;

    u.fix = fixd;
    tmp = (u.value & 0xffff0000) >> 16;

    while(u.value) {
      u.value &= 0xffff;
      if(u.value & 0x8000)tmp += 1/exp;
      exp *= 2;
      u.value <<= 1;
    }

  return (tmp);
}
