#ifndef TIPE_H
#define TIPE_H

#include <stdlib.h>
#include <stdbool.h>

#include <semaphore.h>

typedef struct tipe tipe;

struct tipe
{
    bool
        open;

    size_t
        writepos,
        readpos,
        size,
        nmemb;

    unsigned char
        *buf;

    sem_t
        fill[1],
        empty[1],
        mutex[1];
};


int tipe_init(tipe *t, size_t size, size_t nmemb);
void tipe_destroy(tipe *t);

void tipe_close(tipe *t);
void tipe_open(tipe *t);

int tipe_write(tipe *t, void *obj);
int tipe_read(tipe *t, void *obj);


#endif
