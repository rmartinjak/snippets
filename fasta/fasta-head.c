#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define NDEFAULT 10

void fasta_print(FILE *f, int n)
{
    int c, next;
    while (n--)
    {
        while ((c = fgetc(f)) != EOF)
        {
            putchar(c);
            if (c == '\n')
            {
                next = fgetc(f);
                ungetc(next, f);
                if (next == '>' || next == EOF)
                    break;
            }
        }
    }
}

int main(int argc, char **argv)
{
    int read_stdin = 1;
    int n = NDEFAULT;
    int i;

    for (i = 1; i < argc; i++)
    {
        if (!strcmp(argv[i], "-n"))
        {
            char *endptr;
            if (!argv[++i] || (n = strtol(argv[i], &endptr, 10)) < 0 || *endptr)
            {
                fprintf(stderr, "-n requires a non-negative integer argument\n");
                return EXIT_FAILURE;
            }
        }
        else
            read_stdin = 0;
    }

    if (read_stdin)
    {
        fasta_print(stdin, n);
        return EXIT_SUCCESS;
    }

    for (i = 1; i < argc; i++)
    {
        if (!strcmp(argv[i], "-n"))
        {
            i++;
            continue;
        }
        else
        {
            FILE *f = fopen(argv[i], "r");
            if (!f)
            {
                fprintf(stderr, "%s: ", argv[i]);
                perror("");
                return EXIT_FAILURE;
            }
            fasta_print(f, n);
            fclose(f);
        }
    }

    return EXIT_SUCCESS;
}
