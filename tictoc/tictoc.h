#ifndef TICTOC_H
#define TICTOC_H

#include <stdio.h>
#include <time.h>

#define TICTOC_MAX 10

#define TIC() tictoc(NULL, NULL)
#define FTOC(str, f) tictoc(str, f)
#define TOC(str) FTOC(str, stderr)

static void tictoc(const char *toc_str, FILE *f)
{
    static size_t i;
    static struct timespec tic[TICTOC_MAX], toc;

    if (!toc_str)
    {
        clock_gettime(CLOCK_REALTIME, &tic[i]);

        if (i < TICTOC_MAX)
            i++;
    }
    else
    {
        double d;
        clock_gettime(CLOCK_REALTIME, &toc);

        if (i > 0)
            i--;

        d = (toc.tv_sec - tic[i].tv_sec) + (toc.tv_nsec - tic[i].tv_nsec) / 1e9;
        fprintf(f, "%-10s\t: %f\n", toc_str, d);
    }
}

#endif
