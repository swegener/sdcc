/** bug-3822.c: Function pointer declaration following a block erased variable declaration within.
 */

#include <testfwk.h>

void f1(void) {

    for (;;) {
        char c;  // this declaration got erased
        c;  // this became an unknown identifier
    }

    void (*fun)(int i);
    fun;

    int i;  // make sure that function parameters do not leak into this scope
    i;
}

void f2(void) {

    if (1) {  // also affected "if" since the implementation of if-declarations
        char c;  // this declaration got erased
        c;  // this became an unknown identifier
    }

    void (*fun)(int i);
    fun;

    int i;  // make sure that function parameters do not leak into this scope
    i;
}
