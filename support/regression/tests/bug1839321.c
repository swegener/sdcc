/*
    bug 1839321
*/

#include <testfwk.h>

xdata char Global = 2;

code struct Value {
  char xdata * Name[2];
} Value_1 = {{&Global, 0}},
  Value_2 = {{&Global, 0}};

char i = 1;

// note: this function expects its first parameter to be passed in
//        2 bytes on **stack** (not registers)
char bar(char xdata* code* ptr, ...)
{
	return **ptr;
}

void foo (void) {
}


void
testBug(void)
{
	ASSERT (bar(i ? Value_1.Name : Value_2.Name) == 2);
}
