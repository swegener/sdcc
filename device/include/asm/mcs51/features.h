/** MCS51 specific features.
 */
#ifndef __SDC51_ASM_MCS51_FEATURES_H
#define __SDC51_ASM_MCS51_FEATURES_H   1

#define _REENTRANT	reentrant
#define _CODE		code

/* define _AUTOMEM to indicate the default storage class for
   automatic variables. To be used with pointers to automatic variables.
   Cannot be used in reentrant declared functions!

   void foo(void)
   {
     char Ar[10];
     char _AUTOMEM * pAr = Ar;
   }
*/
#if defined(SDCC_STACK_AUTO)
  #if defined(SDCC_USE_XSTACK)
    #define _AUTOMEM pdata
  #else
    #define _AUTOMEM idata
  #endif
#elif defined(SDCC_MODEL_SMALL)
  #define _AUTOMEM data
#else
  #define _AUTOMEM xdata
#endif

/* define _STATMEM to indicate the default storage class for
   global/static variables. To be used with pointers to static variables.

   char Ar[10];
   void foo(void)
   {
     char _STATMEM * pAr = Ar;
   }
*/
#if defined(SDCC_MODEL_SMALL)
  #define _STATMEM data
#else
  #define _STATMEM xdata
#endif

#endif
