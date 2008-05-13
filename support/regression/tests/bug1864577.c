/*
    bug 1864577
*/

#include <testfwk.h>

#ifdef SDCC
#pragma std_sdcc99
#endif

typedef unsigned char uint8_t;

typedef uint8_t error_t  ;

enum __nesc_unnamed4247 {
  SUCCESS = 0, 
  FAIL = 1, 
  ESIZE = 2, 
  ECANCEL = 3, 
  EOFF = 4, 
  EBUSY = 5, 
  EINVAL = 6, 
  ERETRY = 7, 
  ERESERVE = 8, 
  EALREADY = 9
};

static inline   error_t PlatformP__LedsInit__default__init(void) {
  return SUCCESS;
}

static  error_t PlatformP__LedsInit__init(void);
static inline  error_t PlatformP__LedsInit__init(void){
  unsigned char result;

  result = PlatformP__LedsInit__default__init();

  return result;
}

void
testBug(void)
{
  ASSERT (PlatformP__LedsInit__init() == SUCCESS);
}
