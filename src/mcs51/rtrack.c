/*-------------------------------------------------------------------------
  rtrack.c - tracking content of registers on an mcs51

  Copyright 2007 Frieder Ferlemann (Frieder Ferlemann AT web.de)

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
-------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------
  Status:
    - passes regression test suite, still bugs are likely
    - only active if environment variable SDCC_RTRACK is set

  Missed opportunities:
    - does not track symbols as in "mov a,#_my_int" or "mov a,#(_my_int+1)"
    - only used with moves to acc so chances to use: "inc dptr",
      "inc r2", "add a,r2" or "mov r2,a" would not be detected)
    - a label causes loss of tracking (no handling of information of blocks
      known to follow/preceed the current block)
    - not used in aopGet or genRet
    - does not track which registers are known to be unchanged within
      a function (would not have to be saved when calling the function)
-------------------------------------------------------------------------*/


#include <stdio.h>
#include <string.h>
#include "SDCCglobl.h"

#include "common.h"
#include "ralloc.h"
#include "gen.h"

#define DEBUG(x)
//#define DEBUG(x) x

#define D(x) do if (options.verboseAsm) {x;} while(0)

#define REGS8051_SET(idx,val) do{ \
                                  regs8051[idx].value = (val) & 0xff; \
                                  regs8051[idx].valueKnown = 1; \
                                  DEBUG(printf("%s:0x%02x\n",regs8051[idx].name, \
                                                       regs8051[idx].value);) \
                              } while(0)

#define REGS8051_UNSET(idx)   do{ \
                                  regs8051[idx].valueKnown = 0; \
                                  DEBUG(printf("%s:*\n",regs8051[idx].name);) \
                              } while(0)

/* r0..r7 are not in numerical order in struct regs: r2..r7,r0,r1 */
#define Rx_NUM_TO_IDX(num) (R2_IDX+((num-2)&0x07))


/* move this (or rtrackGetLit() and rtrackMoveALit()
   elsewhere? stealing emitcode from gen.c */
void emitcode (const char *inst, const char *fmt,...);


static int enable = -1;

/*
static void dumpAll()
{
  unsigned int i;
  unsigned int nl=0;

  for (i=0; i<END_IDX; i++)
    {
       if (regs8051[i].valueKnown)
         {
           if (!nl)
             {
               DEBUG(printf("know:");)
             }
           DEBUG(printf(" %s:0x%02x",regs8051[i].name,regs8051[i].value);)
           nl = 1;
         }
    }
  if (nl)
    {
      DEBUG(printf("\n");)
    }
}
*/


static void invalidateAllRx()
{
  //DEBUG(dumpAll();)
  DEBUG(printf("R0..7:*\n");)
  regs8051[R2_IDX].valueKnown = 0;
  regs8051[R3_IDX].valueKnown = 0;
  regs8051[R4_IDX].valueKnown = 0;
  regs8051[R5_IDX].valueKnown = 0;
  regs8051[R6_IDX].valueKnown = 0;
  regs8051[R7_IDX].valueKnown = 0;
  regs8051[R0_IDX].valueKnown = 0;
  regs8051[R1_IDX].valueKnown = 0;
}

static void invalidateAll()
{
  DEBUG(printf("All:* ");)
  invalidateAllRx();
  regs8051[DPL_IDX].valueKnown = 0;
  regs8051[DPH_IDX].valueKnown = 0;
  regs8051[B_IDX].valueKnown = 0;
  regs8051[A_IDX].valueKnown = 0;
}

static regs * getReg(const char *str)
{
  char *s;
  int regNum;

  regNum = strtol (str, &s, 16);
  if (s == str+1)
    {
      return &regs8051[Rx_NUM_TO_IDX(regNum)];
    }
  return NULL;
}

/* tracking values within registers by looking
   at the line passed to the assembler.
   Tries to keep regs8051[] up to date */
void rtrackUpdate (const char *line)
{
  if (enable == -1)
    enable = (NULL != getenv("SDCC_RTRACK"));

  if (!enable ||
      *line == ';' ||                 /* comment */
      (NULL != strstr( line, "==."))) /* dirty check for _G.debugLine */
    return;                           /* nothing to do */

  DEBUG(printf("%s\n",line);)

  if (!strncmp (line,"mov",3))
    {
      /* check literal mov to accumulator */
      if(!strncmp (line,"mov\ta,#0x",9))
        {
          char *s;
          int value;

          value = strtol (line+7, &s, 16);
          if (s != line+7)
              REGS8051_SET (A_IDX, value); /* valid hex found */
          else
              REGS8051_UNSET (A_IDX); /* probably a symbol (not handled) */

          return;
        }

      if (!strncmp (line,"mov\ta,r",7))
        {
          /* handle mov from Rx if Rx is known */
          regs *r = getReg(line+7);
          if (r && r->valueKnown)
            {
              REGS8051_SET (A_IDX, r->value);
              return;
            }
          REGS8051_UNSET (A_IDX);
          return;
        }

      if (!strncmp (line,"mov\ta",5))
        {
          REGS8051_UNSET (A_IDX);
          return;
        }

      if (!strncmp (line,"movc\ta",6) ||
          !strncmp (line,"movx\ta",6))
        {
          REGS8051_UNSET (A_IDX);
          return;
        }

      /* move direct to symbol, do not care */
      if (!strncmp (line,"mov\t_",5) ||
          !strncmp (line,"mov\t(_",6))
        return;

      /* check literal mov to register */
      if (!strncmp (line,"mov\tr",5))
        {
          char *s;
          int value;
          int regNum;

          regNum = strtol (line+5, &s, 16);
          if (s == line+6)
            {
              value = strtol (line+8, &s, 16);
              if ((s != line+8) && !strncmp (line+6,",#0x",4))
                REGS8051_SET (Rx_NUM_TO_IDX(regNum), value);
              else
                REGS8051_UNSET (Rx_NUM_TO_IDX(regNum));
              return;
            }
        }

      /* mov to psw can change register bank */
      if (!strncmp (line,"mov\tpsw,",8))
        {
          invalidateAllRx();
          return;
        }

      /* no tracking of these, so we do not care */
      if (!strncmp (line,"mov\tdptr,#",10) ||
          !strncmp (line,"mov\tdpl,",8) ||
          !strncmp (line,"mov\tdph,",8) ||
          !strncmp (line,"mov\tsp,",7) ||
          !strncmp (line,"mov\tb,",6))
        return;

      /* mov to xdata memory does not change registers */
      if (!strncmp (line,"movx\t@",6))
        return;

      if (!strncmp (line,"mov\t@",5))
        {
          invalidateAllRx();
          return;
        }
    }

  /* no tracking of SP */
  if (!strncmp (line,"push",4))
    return;

  if (!strncmp (line,"pop\ta",5))
    {
      if (!strncmp (line+4,"acc",3)){ REGS8051_UNSET (A_IDX); return; }
      if (!strncmp (line+4,"ar2",3)){ REGS8051_UNSET (Rx_NUM_TO_IDX (2)); return; }
      if (!strncmp (line+4,"ar3",3)){ REGS8051_UNSET (Rx_NUM_TO_IDX (3)); return; }
      if (!strncmp (line+4,"ar4",3)){ REGS8051_UNSET (Rx_NUM_TO_IDX (4)); return; }
      if (!strncmp (line+4,"ar5",3)){ REGS8051_UNSET (Rx_NUM_TO_IDX (5)); return; }
      if (!strncmp (line+4,"ar6",3)){ REGS8051_UNSET (Rx_NUM_TO_IDX (6)); return; }
      if (!strncmp (line+4,"ar7",3)){ REGS8051_UNSET (Rx_NUM_TO_IDX (7)); return; }
      if (!strncmp (line+4,"ar0",3)){ REGS8051_UNSET (Rx_NUM_TO_IDX (0)); return; }
      if (!strncmp (line+4,"ar1",3)){ REGS8051_UNSET (Rx_NUM_TO_IDX (1)); return; }
    }

  if (!strncmp (line,"inc",3))
    {
      /* no tracking of dptr, ignore */
      if (!strcmp (line,"inc\tdptr") ||
          !strcmp (line,"inc\tdph") ||
          !strcmp (line,"inc\tdpl"))
        return;

      if (!strcmp (line,"inc\ta"))
        {
          if (regs8051[A_IDX].valueKnown)
            REGS8051_SET (A_IDX, regs8051[A_IDX].value+1);
          return;
        }

      if(!strncmp (line,"inc\tr",5))
        {
          regs *r = getReg(line+5);
          if (r && r->valueKnown)
            {
              REGS8051_SET (r->rIdx, r->value+1);
            }
          return;
        }
    }

  /* some bit in acc is cleared
     MB: I'm too lazy to find out which right now */
  if (!strncmp (line,"jbc\tacc",7))
    {
      REGS8051_UNSET (A_IDX);
      return;
    }

  /* unfortunately the label typically following these
     will cause loss of tracking */
  if (!strncmp (line,"jc\t",3) ||
      !strncmp (line,"jnc\t",4) ||
      !strncmp (line,"jb\t",3) ||
      !strncmp (line,"jnb\t",4) ||
      !strncmp (line,"jbc\t",4))
    return;

  /* if branch not taken in "cjne r2,#0x08,somewhere" 
     r2 is known to be 8 */
  if (!strncmp (line,"cjne",4))
    {
      if(!strncmp (line,"cjne\ta,#0x",10))
        {
          char *s;
          int value;

          value = strtol (line+8, &s, 16);
          if (s != line+8)
              REGS8051_SET (A_IDX, value); /* valid hex found */
        }
      if(!strncmp (line,"cjne\tr",6))
        {
          char *s;
          int value;
          regs *r = getReg(line+6);
          value = strtol (line+8, &s, 16);
          if (r && s != line+8)
              REGS8051_SET (r->rIdx, value); /* valid hex found */
        }
      return;
    }

  /* acc eventually known to be zero */
  if (!strncmp (line,"jz\t",3))
    return;

  /* acc eventually known to be zero */
  if (!strncmp (line,"jnz\t",4))
    {
      REGS8051_SET (A_IDX, 0x00); // branch not taken
      return;
    }

  if (!strncmp (line,"djnz\tr",6))
    {
      char *s;
      int regNum;

      regNum = strtol (line+6, &s, 16);
      if (s == line+7)
        {
          //REGS8051_UNSET (Rx_NUM_TO_IDX(regNum));
          REGS8051_SET (Rx_NUM_TO_IDX(regNum), 0x00); // branch not taken
          return;
        }
    }

  /* only carry bit, so we do not care */
  if (!strncmp (line,"setb\tc",6) ||
      !strncmp (line,"clr\tc",5) ||
      !strncmp (line,"cpl\tc",5))
    return;

  if (!strncmp (line,"add\ta,",6) ||
      !strncmp (line,"addc\ta,",7)||
      !strncmp (line,"subb\ta,",7)||
      !strncmp (line,"xrl\ta,",6) ||
      !strncmp (line,"orl\ta,",6) ||
      !strncmp (line,"anl\ta,",6) ||
      !strncmp (line,"da\ta",4)   ||
      !strncmp (line,"rlc\ta,",6) ||
      !strncmp (line,"rrc\ta,",6) ||
      !strncmp (line,"setb\ta",6) ||
      !strncmp (line,"clrb\ta,",7)||
      !strncmp (line,"cpl\tacc",7))
    {
      /* could also handle f.e. "add a,Rx" if a, Rx are known or "xrl a,#0x08" */
      REGS8051_UNSET (A_IDX);
      return;
    }


  if (!strncmp (line,"dec",3))
    {
      /* no tracking of dptr, so we would not care */
      if (!strcmp (line,"dec\tdph") ||
          !strcmp (line,"dec\tdpl"))
        return;

      if (!strcmp (line,"dec\ta"))
        {
          if (regs8051[A_IDX].valueKnown)
            REGS8051_SET (A_IDX, regs8051[A_IDX].value-1);
          return;
        }

      if(!strncmp (line,"dec\tr",5))
        {
          regs *r = getReg(line+5);
          if (r && r->valueKnown)
            {
              REGS8051_SET (r->rIdx, r->value-1);
            }
          return;
        }
    }


  if (!strcmp (line,"clr\ta"))
    {
      REGS8051_SET (A_IDX, 0);
      return;
    }

  if (!strcmp (line,"cpl\ta"))
    {
      if (regs8051[A_IDX].valueKnown)
        REGS8051_SET (A_IDX, ~regs8051[A_IDX].value);
      return;
    }
  if (!strcmp (line,"rl\ta"))
    {
      if (regs8051[A_IDX].valueKnown)
        REGS8051_SET (A_IDX, (regs8051[A_IDX].value<<1) | 
                             (regs8051[A_IDX].value>>7) );
      return;
    }
  if (!strcmp (line,"rr\ta"))
    {
      if (regs8051[A_IDX].valueKnown)
        REGS8051_SET (A_IDX, (regs8051[A_IDX].value>>1) |
                             (regs8051[A_IDX].value<<7));
      return;
    }
  if (!strcmp (line,"swap\ta"))
    {
      if (regs8051[A_IDX].valueKnown)
        REGS8051_SET (A_IDX, (regs8051[A_IDX].value>>4) |
                             (regs8051[A_IDX].value<<4));
      return;
    }

  if (!strncmp (line,"mul",3) ||
      !strncmp (line,"div",3)
      )
    {
      REGS8051_UNSET (A_IDX);
      REGS8051_UNSET (B_IDX);
      return;
    }

  /* assuming these library functions have no side-effects */
  if (!strcmp (line,"lcall"))
    {
      if (!strcmp (line,"lcall\t__gptrput"))
        {
          /* invalidate R0..R7 because they might have been changed */
          /* MB: too paranoid ? */
          //invalidateAllRx();
          return;
        }
      if (!strcmp (line,"lcall\t__gptrget"))
        {
          REGS8051_UNSET (A_IDX);
          return;
        }
      if (!strcmp (line,"lcall\t__decdptr"))
        {
          return;
        }
     }

  if (!strncmp (line,"xch\ta,r",7))
    {
      /* handle xch acc with Rn */
      regs *r = getReg(line+7);
      if (r)
        {
          unsigned swap;
          swap = r->valueKnown;
          r->valueKnown = regs8051[A_IDX].valueKnown;
          regs8051[A_IDX].valueKnown = swap;

          swap = r->value;
          r->value = regs8051[A_IDX].value;
          regs8051[A_IDX].value = swap;
          return;
        }
    }

  /* all others unrecognized, invalidate */
  invalidateAll();
}


/* expects f.e. "#0x01" and returns either "#0x01"
   if the value is not known to be within registers
   or "a" or "r0".."r7".
   (mov a,r7 or add a,r7 need one byte whereas
    mov a,#0x01 or add a,#0x01 would take two
 */
char * rtrackGetLit(const char *x)
{
  unsigned int i;

  char *s;

  if (enable != 1)
    return (char *)x;

  /* was it a numerical literal? */
  if (*x == '#')
    {
      int val = strtol (x+1, &s, 16);
      if (x+1 != s)
        {
          /* try to get from acc */
          regs *r = &regs8051[A_IDX];
          if (r->valueKnown &&
              r->value == val)
            {
              D(emitcode (";", "genFromRTrack 0x%02x=%s", val, r->name));
              return r->name;
            }
          /* try to get from register R0..R7 */
          for (i=0; i<8; i++)
            {
              regs *r = &regs8051[Rx_NUM_TO_IDX(i)];
              if (r->valueKnown &&
                  r->value == val)
                {
                  D(emitcode (";", "genFromRTrack 0x%02x=%s", val, r->name));
                  return r->name;
                }
            }
        }
      else
        {
          /* probably a symbolic literal as in "mov r3,#(_i+1)",
             not handled... */
        }
    }

  return (char *)x;
}

/* Similar to the above function 
   As the destination is the accumulator try harder yet and
   try to generate the result with arithmetic operations */
int rtrackMoveALit (const char *x)
{

  if (enable != 1)
    return 0;

  /* if it is a literal mov try to get it cheaper */
  if ( *x == '#' )
    {
      regs *a = &regs8051[A_IDX];

      char *s;
      int val = strtol (x+1, &s, 16);

      /* was it a numerical literal? */
      if (x+1 != s)
        {
          /* prefer mov a,#0x00 */
          if (val == 0 &&
              ((a->valueKnown && a->value != 0) ||
               !a->valueKnown))
            {
              /* peepholes convert to clr a */
              /* MB: why not here ? */
              emitcode ("mov", "a,#0x00");
              return 1;
            }

          if (a->valueKnown)
            {
              /* already there? */
              if (val == a->value)
                {
                  D(emitcode (";", "genFromRTrack acc=0x%02x", a->value));
                  return 1;
                }

              /* can be calculated with an instruction
                 that does not change flags from acc itself? */
              if (val == ((a->value+1) & 0xff) )
                {
                  D(emitcode (";", "genFromRTrack 0x%02x=0x%02x+1", val, a->value));
                  emitcode ("inc", "a");
                  return 1;
                }
              if (val == ((a->value-1) & 0xff) )
                {
                  D(emitcode (";", "genFromRTrack 0x%02x=0x%02x-1", val, a->value));
                  emitcode ("dec", "a");
                  return 1;
                }
              if (val == ((~a->value) & 0xff) )
                {
                  D(emitcode (";", "genFromRTrack 0x%02x=~0x%02x", val, a->value));
                  emitcode ("cpl", "a");
                  return 1;
                }
              if (val == (((a->value>>1) |
                           (a->value<<7)) & 0xff))
                {
                  D(emitcode (";", "genFromRTrack 0x%02x=rr(0x%02x)", val, a->value));
                  emitcode ("rr", "a");
                  return 1;
                }
              if (val == (((a->value<<1) |
                           (a->value>>7)) & 0xff ))
                {
                  D(emitcode (";", "genFromRTrack 0x%02x=rl(0x%02x)", val, a->value));
                  emitcode ("rl", "a");
                  return 1;
                }
              if (val == ( ((a->value & 0x0f)<<4) |
                           ((a->value & 0xf0)>>4) ))
                {
                  D(emitcode (";", "genFromRTrack 0x%02x=swap(0x%02x)", val, a->value));
                  emitcode ("swap", "a");
                  return 1;
                }
              /* Decimal Adjust Accumulator (da a) changes flags so not used */
            }


          {
            unsigned int i;
            char *ptr= rtrackGetLit(x);

            if (x != ptr)
              {
                /* could get from register, fine */
                emitcode ("mov", "a,%s", ptr);
                return 1;
              }

            /* not yet giving up - try to calculate from register R0..R7 */
            for (i=0; i<8; i++)
              {
                regs *r = &regs8051[Rx_NUM_TO_IDX(i)];

                if (a->valueKnown && r->valueKnown)
                  {
                    /* calculate with a single byte instruction from R0..R7? */
                    if (val == (a->value | r->value))
                      {
                        D(emitcode (";", "genFromRTrack 0x%02x=0x%02x|0x%02x",
                                    val, a->value, r->value));
                        emitcode ("orl", "a,%s",r->name);
                        return 1;
                      }
                    if (val == (a->value & r->value))
                      {
                        D(emitcode (";", "genFromRTrack 0x%02x=0x%02x&0x%02x",
                                    val, a->value, r->value));
                        emitcode ("anl", "a,%s", r->name);
                        return 1;
                      }
                    if (val == (a->value ^ r->value))
                      {
                        D(emitcode (";", "genFromRTrack 0x%02x=0x%02x^0x%02x",
                                    val, a->value, r->value));
                        emitcode ("xrl", "a,%s", r->name);
                        return 1;
                      }
                    /* changes flags (does that matter?)
                    if (val == (a->value + r->value))
                      {
                        D(emitcode (";", "genFromRTrack 0x%02x=0x%02x+%0x02x",
                                    val, a->value, r->value));
                        emitcode ("add", "a,%s",r->name);
                        return 1;
                      }
                    so not used */
                  }
              }
          }
      }
    }

  return 0;
}

