#ifndef PPOPTS_H
#define PPOPTS_H


#define PPOPTS_NO_LONGOPTS (1 << 0)
#define PPOPTS_DESC_ON_NEXT_LINE (1 << 1)


#define PPOPTS_LONGOPT_MAX 30
#define PPOPTS_ARGNAME_MAX 10
#define PPOPTS_DESC_MAX 4096
#define PPOPTS_OPTS_MAX 30


struct ppopts
{
    struct ppopts_opt {
        char shortopt;
        char longopt[PPOPTS_LONGOPT_MAX + 1];
        char argname[PPOPTS_ARGNAME_MAX + 1];
        char desc[PPOPTS_DESC_MAX + 1];
    } options[PPOPTS_OPTS_MAX];
    int maxlen_longopt, maxlen_argname;
    int n;
};


#define PPOPTS_INITIALIZER { {{ 0, "", "", "" }}, 0, 0, 0 }


void ppopts_init(struct ppopts *o);


void ppopts_add(struct ppopts *o, char shortopt, const char *longopt,
                const char *argname, const char *desc);


void ppopts_print(struct ppopts *o, FILE *stream, int wrap, int flags);
#endif
