#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>
#include <string.h>

#include "ppopts.h"

#define FMT_SHORT "-%c %-*s"
#define FMT_SHORT_WIDTH 3
#define FMT_LONG  "  --%-*s %-*s"
#define FMT_LONG_WIDTH 5
#define SPACE_BEFORE_DESC 4


void
ppopts_init(struct ppopts *o)
{
    memset(o, 0, sizeof *o);
}


void
ppopts_add(struct ppopts *o, int shortopt, const char *longopt,
           const char *argname, const char *desc, ...)
{
    int len;
    struct ppopts_opt *opt;
    va_list ap;

    if (o->n >= PPOPTS_OPTS_MAX) {
        return;
    }
    opt = &o->options[o->n];
    opt->shortopt = shortopt;

    va_start(ap, desc);
    vsnprintf(opt->desc, sizeof opt->desc, desc, ap);
    va_end(ap);

    strncpy(opt->longopt, longopt, PPOPTS_LONGOPT_MAX);
    len = strlen(longopt);
    if (len > o->maxlen_longopt) {
        o->maxlen_longopt = len;
    }

    strncpy(opt->argname, argname, PPOPTS_ARGNAME_MAX);
    len = strlen(argname);
    if (len > o->maxlen_argname) {
        o->maxlen_argname = len;
    }

    o->n++;
}


static int
get_word(char *dest, int n, char *src, char **next)
{
    int len = 0;
    char *p = src;
    while (*p && !isspace(*p)) {
        if (p - src < n - 1) {
            *dest++ = *p;
            len++;
        }
        p++;
    }
    if (*p) {
        *dest++ = ' ';
        len++;
    }
    *dest = '\0';
    while (*p && isspace(*p)) {
        p++;
    }
    *next = p;
    return len;
}


static void
print_desc_literal(FILE *stream, char *desc)
{
    char *s = desc;
    fputc('\n', stream);
    for (; *s; s++) {
        if (s == desc || s[-1] == '\n') {
            fprintf(stream, "%*s", SPACE_BEFORE_DESC, "");
        }
        fputc(*s, stream);
    }
    fputc('\n', stream);
}


static void
print_desc(FILE *stream, char *desc, int indent, int wrap)
{
    char *s = desc;
    int width = indent >= 0 ? wrap - indent : wrap - SPACE_BEFORE_DESC;

    while (*s) {
        char word[PPOPTS_DESC_MAX];
        char *p = s, *next;
        int word_len, line_len = 0;
        if (s != desc || indent < 0) {
            if (indent < 0) {
                fputc('\n', stream);
                indent = SPACE_BEFORE_DESC;
            }
            fprintf(stream, "%*s", indent, "");
        }
        while (*p && (line_len < width || !line_len)) {
            word_len = get_word(word, sizeof word, p, &next);
            if (line_len && line_len + word_len >= width) {
                break;
            }
            fprintf(stream, "%s", word);
            line_len += word_len;
            p = next;
        }
        fputc('\n', stream);
        s = p;
    }
}


void
ppopts_print(struct ppopts *o, FILE *stream, int wrap, int flags)
{
    int i, w_long = o->maxlen_longopt, w_arg = o->maxlen_argname;
    int indent;
    if (flags & PPOPTS_DESC_ON_NEXT_LINE) {
        indent = -1;
    }
    else {
        indent = SPACE_BEFORE_DESC + FMT_SHORT_WIDTH + w_arg;
        if (!(flags & PPOPTS_NO_LONGOPTS)) {
            indent += FMT_LONG_WIDTH + w_long + w_arg;
        }

        /* no space for desc -> always put it on the next line */
        if (indent  > wrap) {
            wrap = indent - SPACE_BEFORE_DESC;
            indent = -1;
        }
    }

    for (i = 0; i < o->n; i++) {
        struct ppopts_opt *opt = &o->options[i];

        switch (opt->shortopt) {

        case PPOPTS_HEADER:
            fputc('\n', stream);
        case PPOPTS_TEXT:
            print_desc(stream, opt->desc, 0, wrap);
            break;

        default:
            fprintf(stream, FMT_SHORT, opt->shortopt, w_arg, opt->argname);
            if (!(flags & PPOPTS_NO_LONGOPTS)) {
                fprintf(stream, FMT_LONG,
                        w_long, opt->longopt, w_arg, opt->argname);
            }
            fprintf(stream, "%*s", SPACE_BEFORE_DESC, "");
            if (strchr(opt->desc, '\n')) {
                print_desc_literal(stream, opt->desc);
            }
            else {
                print_desc(stream, opt->desc, indent, wrap);
            }
        }

        if (i < o->n - 1 && opt->shortopt != PPOPTS_HEADER) {
            fputc('\n', stream);
        }
    }
}


#ifdef PPOPTS_TEST
int main(void)
{
    struct ppopts opt = PPOPTS_INITIALIZER;
    ppopts_add_header(&opt, "Some options:");
    ppopts_add(&opt, 'o', "option", "ARG", "Some long and very descriptive text. I hope it is long enough to wrap");
    ppopts_add(&opt, 'l', "longeroption", "LONGERARG", "Another long and very descriptive text. I hope it is long enough to wrap even if we use PPOPTS_DESC_ON_NEXT_LINE.");
    ppopts_add(&opt, 's', "super", "FOO", "This is another supercalifragilisticexpialidocious option! Yeah");
    ppopts_add_text(&opt, "FOO should be < %d", 3);
    ppopts_add(&opt, 'n', "newline", "X", "This option contains a newline:\nit shall always be printed literally (including   all  \t whitespace).");
    ppopts_print(&opt, stdout, 80, 0);
    printf("\n\n");
    ppopts_print(&opt, stdout, 80, PPOPTS_NO_LONGOPTS);
    printf("\n\n");
    ppopts_print(&opt, stdout, 80, PPOPTS_DESC_ON_NEXT_LINE);
    return EXIT_SUCCESS;
}
#endif
