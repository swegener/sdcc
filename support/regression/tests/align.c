/** Tests covering alignment operators.

    sign: signed, unsigned
    type: char, int, long
*/

#include <testfwk.h>

#include <stddef.h>
#include <stdalign.h>

void
testAlignof(void)
{
  ASSERT(alignof(char)  <= alignof({sign} {type}));
  /*ASSERT(alignof({sign} {type})  <= alignof(max_align_t)); #pragma std_c11 support incomplete */

  /* sdcc currently only architectures that do not have alignment restrictions. */
#ifndef PORT_HOST
  ASSERT(alignof({sign} {type})  == 1);
  /*ASSERT(alignof(max_align_t)  == 1); #pragma std_c11 support incomplete */
#endif
}

