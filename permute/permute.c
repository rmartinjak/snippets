#include <stdlib.h>
#include <stdio.h>
#include <time.h>

void permute(int *x, int n)
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

#ifdef PERMUTE_TEST
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
#endif
