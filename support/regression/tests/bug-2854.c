/* bug-2854.c
   A bug in handling of 8-bit parameters to z88dk_fastcall. Also bug #2852.
 */

#include <testfwk.h>

#if 0

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#pragma disable_warning 85
#pragma disable_warning 88
#pragma disable_warning 127

void lock_give_fastcall(uint8_t * mutex)
#ifdef __SDCC_z80
__preserves_regs(b,c,d,e,iyh,iyl) __z88dk_fastcall
#endif
{
}
#define lock_give(a) lock_give_fastcall(a)

uint8_t bank_get_abs_fastcall(int8_t bankRel)
#ifdef __SDCC_z80
__preserves_regs(b,c,d,e,h,iyh,iyl) __z88dk_fastcall
#endif
{
  ASSERT (bankRel = 23);
  return 0;
}
#define bank_get_abs(a) bank_get_abs_fastcall(a)

// provide memcpy_far function
void *memcpy_far(void *str1,int8_t bank1,const void *str2,const int8_t bank2,size_t n)
#ifdef __SDCC_z80
__preserves_regs(iyh,iyl)
#endif
{
}

// provide the simple mutex locks for the BANK (for system usage)
uint8_t bankLockBase[16];  // base address for 16 BANK locks

static void *buffer;            /* create a scratch buffer on heap later */

/**
   @brief Builtin command:
   @param args List of args.  args[0] is "mvb".  args[1] is source bank. args[2] is the destination bank.
   @return Always returns 1, to continue executing.
 */
int8_t ya_mvb(char **args)      // move or clone the nominated bank
{
    if ( (args[2] != NULL) && (bank_get_abs((int8_t)atoi(args[1])) != 0) && (bank_get_abs((int8_t)atoi(args[2])) != 0) )   // the source and destination can never be BANK0
    {
        // do the copy
        memcpy_far((void *)0x0000, (int8_t)atoi(args[2]), (void *)0x0000, (int8_t)atoi(args[1]), (0xF000)); // copy it all
        // set bank referenced from _bankLockBase, so the clone bank is noted as the same state as its parent.
        bankLockBase[ bank_get_abs((int8_t)atoi(args[2])) ] = bankLockBase[ bank_get_abs((int8_t)atoi(args[1])) ];
    }
    return 1;
}

/**
   @brief Builtin command:
   @param args List of args.  args[0] is "loadb".  args[1] is the path.
        args[2] is the nominated bank. args[3] is the origin address.
   @return Always returns 1, to continue executing.
 */
int8_t ya_loadb(char **args)    // load the nominated bank and address with binary code
{
    uint8_t * dest;
    uint32_t p1;
    uint16_t s1 = 0;

    if (args[1] == NULL || args[2] == NULL) {
    } else {
        if (args[3] == NULL) {
            dest = (uint8_t *)0x0100;
        } else {
            dest = (uint8_t *)strtoul(args[3], NULL, 16);
        }
        p1 = 0;
        while ((uint16_t)dest < (0xF000)) {
            if (s1 == 0) break;   /* error or eof */

            if (s1 > (0xF000) - (uint16_t)dest) {       // don't overwrite COMMON AREA 1
                s1 = (0xF000) - (uint16_t)dest;
            }
            memcpy_far((void *)dest, (int8_t)atoi(args[2]), buffer, 0, s1);     // write s1 bytes to ram
            dest += s1;
            p1 += s1;
        }

        // set bank referenced from _bankLockBase, so the the bank is noted as warm.
        lock_give( &bankLockBase[ bank_get_abs((int8_t)atoi(args[2])) ] );
    }
    return 1;
}

#endif

void
testBug(void)
{
#if 0
  const char *args[] = {"", "", "23"};
  char b;
  buffer = &b;

  ya_mvb(args);

  ya_loadb(args);
#endif
}
