#include "permute.h"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <assert.h>

int main(void)
{
#define N 100
    int x[N], ok[N] = { 0 };
    int i;

    srand(time(NULL));

    permute(x, N);

    for (i = 0; i < N; i++)
        ok[x[i]] = 1;

    for (i = 0; i < N; i++)
        assert(ok[i]);

    puts("test successful");

    return EXIT_SUCCESS;
}
