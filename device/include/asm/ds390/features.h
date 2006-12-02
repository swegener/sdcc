/** DS390/DS400 specific features.
 */
#ifndef __SDC51_ASM_DS390_FEATURES_H
#define __SDC51_ASM_DS390_FEATURES_H   1

#define _REENTRANT	__reentrant
#define _CODE		__code

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
    #define _AUTOMEM __pdata
  #else
    #define _AUTOMEM __idata
  #endif
#elif defined(SDCC_MODEL_SMALL)
  #define _AUTOMEM __data
#else
  #define _AUTOMEM __xdata
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
  #define _STATMEM __data
#else
  #define _STATMEM __xdata
#endif

#endif
