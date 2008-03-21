/*
    bug 1839277
*/

#include <testfwk.h>

code struct Value {
  code char* Name[2];
} Values[2]= {{{"abc", "def"}}, {{"ghi", "jkl"}}};

char i=1;

void
testBug(void)
{
	volatile char code* * p;
	unsigned long v = 0;
//first subexpression 'Values[0].Name' is evaluted as follows:
//mov     r2,#_Values
//mov     r3,#(_Values >> 8)
//mov     r4,#(_Values >> 16) ;this is wrong - should be 'mov r4,#128' shouldn't it?
//second subexpression 'Values[1].Name' is evaluted as follows:
//mov     a,#0x04
//add     a,#_Values
//mov     r2,a
//clr     a
//addc    a,#(_Values >> 8)
//mov     r3,a
//mov     r4,#128 ;this is all right
	p = i ? Values[0].Name : Values[1].Name;
#if defined(SDCC_mcs51)
	v = (unsigned long)p;
	ASSERT((unsigned char)(v>>16)==0x80);
#endif

//everything is all right with explicit typecast - but why do I need it?
	p = i ? (char code**)Values[0].Name : (char code**)Values[1].Name;
#if defined(SDCC_mcs51)
	v = (unsigned long)p;
	ASSERT((unsigned char)(v>>16)==0x80);
#endif

//this is the best/optimal version - again with explicit typecast
//Question: Why is it necessary to have explicit typecast to make things right?
	p = i ? (char code* code*)Values[0].Name : (char code* code*)Values[1].Name;
#if defined(SDCC_mcs51)
	v = (unsigned long)p;
	ASSERT((unsigned char)(v>>16)==0x80);
#endif
}
