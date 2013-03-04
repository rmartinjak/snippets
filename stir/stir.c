#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#define MIN_FILES 2
#define MAX_FILES 100


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

int print_line(FILE *f)
{
    int c;
    if (feof(f))
        return EOF;

    while ((c = fgetc(f)) != EOF && c != '\n')
        putchar(c);
    putchar('\n');

    return c;
}

int main(int argc, char **argv)
{
    FILE *stream[MAX_FILES] = { NULL };
    int i, n;

    srand(time(NULL));

    if (argc < (MIN_FILES + 1) || argc > MAX_FILES)
    {
        fprintf(stderr,
                "number of arguments must be between %d and %d\n",
                MIN_FILES, MAX_FILES);
        return EXIT_FAILURE;
    }

    for (n = 0; n < argc - 1; n++)
    {
        if (!(stream[n] = fopen(argv[n + 1], "r")))
        {
            for (i = 0; i < n; i++)
                fclose(stream[i]);
            fprintf(stderr, "cannot open %s:", argv[n + 1]);
            perror(NULL);
            return EXIT_FAILURE;
        }
    }

    while (1) {
        int printed = 0;
#ifdef SHAKE
        int perm[MAX_FILES];
        permute(perm, n);
#define INDEX(i) (perm[i])
#else
#define INDEX(i) (i)
#endif
        for (i = 0; i < n; i++) {
            if (print_line(stream[INDEX(i)]) != EOF)
                printed = 1;
        }

        if (!printed)
            break;
    }
    for (i = 0; i < n; i++)
        fclose(stream[i]);

    return EXIT_SUCCESS;
}
