#include <stdio.h>
#include <stdarg.h>

typedef struct {
    uint depth; /* Length of dims. */
    uint *dims; /* dims[0]*dims[1]*...*dims[depth-1] is the length of data */
    int  *data;
} intN;

intN allocIntN(uint depth, uint *dims) {

    // uint size = 1;
    // uint i = 0;

    // for (i = 0; i < depth; ++i)
    // {
    //     size *= dims[i];
    // }

    // malloc()

    /* clean memory reserved for valist */

    intN ret;

    ret.depth = depth;
    ret.dims = dims;

    return ret;

}

int main()
{
    uint testDepth = 4;
    uint *testDims = (uint[]) {2, 3, 4, 6};

    allocIntN(testDepth, testDims);

    return 0;
}


