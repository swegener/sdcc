/*
   920625-1.c from the execute part of the gcc torture suite.
 */

#include <testfwk.h>

#ifdef __SDCC
#pragma std_c99
#pragma disable_warning 93
#endif

#include <stdarg.h>

#if !defined(__SDCC_pdk14) // Lack of memory
typedef struct{double x,y;}point;
point pts[]={{1.0,2.0},{3.0,4.0},{5.0,6.0},{7.0,8.0}};
static int va1(int nargs,...)
{
  va_list args;
  int i;
  point pi;
  va_start(args,nargs);
  for(i=0;i<nargs;i++){
    pi=va_arg(args,point);
    if(pts[i].x!=pi.x||pts[i].y!=pi.y)ASSERT(0);
  }
  va_end(args);
}

typedef struct{int x,y;}ipoint;
ipoint ipts[]={{1,2},{3,4},{5,6},{7,8}};
static int va2(int nargs,...)
{
  va_list args;
  int i;
  ipoint pi;
  va_start(args,nargs);
  for(i=0;i<nargs;i++){
    pi=va_arg(args,ipoint);
    if(ipts[i].x!=pi.x||ipts[i].y!=pi.y)ASSERT(0);
  }
  va_end(args);
}
#endif

void
testTortureExecute (void)
{
#if !defined(__SDCC_pdk14) // Lack of memory
#if !defined(__SDCC_stm8) // Bug (assertion fails in code generation)
#if !defined(__SDCC_z80) && !defined(__SDCC_z180) && !defined(__SDCC_sm83) && !defined(__SDCC_ez80_z80) && !defined(__SDCC_r2k) && !defined(__SDCC_r2ka) && !defined(__SDCC_r3ka) && !defined(__SDCC_tlcs90) && !defined(__SDCC_z80n) // Bug
#if !defined(__SDCC_hc08) && !defined(__SDCC_s08) // Bug (fails test)
#if !defined(__SDCC_pdk14) && !defined(__SDCC_pdk15) // Bug (assertion fails in code generation)
va1(4,pts[0],pts[1],pts[2],pts[3]);
va2(4,ipts[0],ipts[1],ipts[2],ipts[3]);
return;
#endif
#endif
#endif
#endif
#endif
}

