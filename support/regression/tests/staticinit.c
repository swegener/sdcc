/** Tests that the static initialiser code works.
    As the init code is now clever we have to be careful.

    type: char, int, long
*/
#include <testfwk.h>

static {type} smallDense[] = {
    1, 2, 3, 4, 5, 6
};

static void
testSmallDense(void)
{
    ASSERT(smallDense[0] == 1);
    ASSERT(smallDense[1] == 2);
    ASSERT(smallDense[2] == 3);
    ASSERT(smallDense[3] == 4);
    ASSERT(smallDense[4] == 5);
    ASSERT(smallDense[5] == 6);
}

static {type} smallSparse[] = {
    1, 1, 1, 1, 1, 1, 1, 1, 1
};

static void
testSmallSparse(void)
{
    ASSERT(smallSparse[0] == 1);
    ASSERT(smallSparse[1] == 1);
    ASSERT(smallSparse[2] == 1);
    ASSERT(smallSparse[3] == 1);
    ASSERT(smallSparse[4] == 1);
    ASSERT(smallSparse[5] == 1);
    ASSERT(smallSparse[6] == 1);
    ASSERT(smallSparse[7] == 1);
    ASSERT(smallSparse[8] == 1);
}

static {type} smallSparseZero[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0
};

