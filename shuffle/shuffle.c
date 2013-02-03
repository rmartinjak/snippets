#include <stdio.h>
#include <stdlib.h>

struct line
{
    long pos;
    size_t len;
    struct line *prev, *next;
};

struct line *line_get(struct line *ln, unsigned n)
{
    while (n--)
        ln = ln->next;
    return ln;
}

struct line *line_add(struct line *ln, long pos, size_t len)
{
    struct line *new = malloc(sizeof *new);
    if (new)
    {
        new->pos = pos;
        new->len = len;

        new->next = ln->next;
        new->prev = ln;

        ln->next->prev = new;
        ln->next = new;
    }
    return new;
}

void line_del(struct line *ln)
{
    ln->prev->next = ln->next;
    ln->next->prev = ln->prev;
}

void shuffle_file(const char *filename)
{
    struct line root;
    int c;
    FILE *f;
    long pos;
    size_t len;

    root.next = &root;
    root.prev = &root;

    f = fopen(filename, "r");
    if (!f) {
        fprintf(stderr, "cannot open %s:", filename);
        perror("");
        return;
    }

    pos = 0;
    len = 1;
    while ((c = fgetc(f)) != EOF) {
        len++;
        if (c == '\n')
        {
            line_add(&root, pos, len);
            pos = ftell(f);
            len = 1;
            c = fgetc(f);
            if (c == EOF)
                break;
        }
    }

    while (root.next != &root) {
        struct line *r = &root;
        struct line *ln = line_get(r, random() % 256);

        if (ln == &root)
            continue;

        fseek(f, ln->pos, SEEK_SET);
        len = ln->len;
        while (len--) {
            c = fgetc(f);
            putchar(c);
        }
        line_del(ln);
        free(ln);
    }
}

int main(int argc, char **argv)
{
    char *fn;
    while ((fn = *++argv))
        shuffle_file(fn);

    return EXIT_SUCCESS;
}