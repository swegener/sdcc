/* bug 460010
 */
#include <testfwk.h>

#if defined __mcs51 || defined __ds390
#define XDATA xdata
#else
#define XDATA
#endif

void 
func( unsigned char a )
{
  UNUSED(a);
}

void
testBadPromotion(void)
{

#ifdef SDCC
  unsigned char c=*((unsigned XDATA char*)(0xa000));
#else
  unsigned char loc_c;
  unsigned char c=*(unsigned char*)&loc_c;
#endif 
  
  func(c); 
  
  c+='0'; /* is evaluated as an 8-bit expr */ 
  
  func(c); 

  c+='A'-'0'; /* is a 16-bit expr ??? */ 
  
  func(c); 
}
