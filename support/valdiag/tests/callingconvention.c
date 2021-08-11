
#if defined(__SDCC_gbz80) && 0
#define has_sdcccall 1
#define SDCCCALL0  __sdcccall(0)
#define SDCCCALL1  __sdcccall(1)
#else
#define SDCCCALL0
#define SDCCCALL1
#endif

#if (defined(__SDCC_z80) || defined(__SDCC_z80n) || defined(__SDCC_z180)) && 0
#define has_z88dk_fastcall 1
#define Z88DK_FASTCALL  __z88dk_fastcall
#else
#define Z88DK_FASTCALL
#endif

#if defined(__SDCC_stm8)
#define has_raisonance 1
#define RAISONANCE  __raisonance
#else
#define RAISONANCE
#endif

#ifdef TEST1
int f(int i) SDCCCALL0;        /* IGNORE */
int f(int i) SDCCCALL1 {       /* ERROR(has_sdcccall) */
  return i;
}
#endif

#ifdef TEST2
int f(int i) SDCCCALL1;        /* IGNORE */
int f(int i) SDCCCALL0 {       /* ERROR(has_sdcccall) */

  return i;
}
#endif

#ifdef TEST3
int f(int i) Z88DK_FASTCALL;   /* IGNORE */
int f(int i) {                 /* ERROR(has_z88dk_fastcall) */
  return i;
}
#endif

#ifdef TEST4
int f(int i);                  /* IGNORE */
int f(int i) Z88DK_FASTCALL {  /* ERROR(has_z88dk_fastcall) */

  return i;
}
#endif

#ifdef TEST5
int f(int i) RAISONANCE;       /* IGNORE */
int f(int i) {                 /* ERROR(has_raisonance) */
  return i;
}
#endif

#ifdef TEST6
int f(int i);                  /* IGNORE */
int f(int i) RAISONANCE {      /* ERROR(has_raisonance) */

  return i;
}
#endif

