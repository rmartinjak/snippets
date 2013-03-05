#include <stdlib.h>

static void permute(int *x, int n)
{
    int i, r;

    for (i = 0; i < n; i++)
        x[i] = -1;

    for (i = 0; i < n; i++)
    {
        for (r = rand() % n; x[r] >= 0; r = (r + 1) % n)
            ;
        x[r] = i;
    }
}
