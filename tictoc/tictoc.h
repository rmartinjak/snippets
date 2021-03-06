/* tictoc.h - a MATLAB-like "stopwatch"
 *
 * Copyright (c) 2014 Robin Martinjak <rob@rmartinjak.de>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See http://www.wtfpl.net/ for more details.
 */

#ifndef TICTOC_H
#define TICTOC_H

/* example:

    #include "tictoc.h"
    #include <stdlib.h>
    #include <time.h>
    #include <unistd.h>

    int main(void)
    {
        int i = 4;
        srand(time(NULL));

        TIC();
        while (i--) {
            TIC();
            sleep(rand() % 3);
            TOC("inner");
        }
        TOC("outer");
        return 0;
    }


  outputs:
    inner       : 2.000181
    inner       : 1.000169
    inner       : 2.000177
    inner       : 0.000003
    outer       : 5.000725
 */

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
