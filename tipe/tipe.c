#include "tipe.h"

#include <stdlib.h>
#include <string.h>
#include <errno.h>

#ifndef DEBUG
#define NDEBUG
#endif

#include <assert.h>


int tipe_init(tipe *t, size_t size, size_t nmemb)
{
    if (nmemb == 0)
    {
        errno = EINVAL;
        return -1;
    }

    if (sem_init(t->fill, 0, 0)) {
        return -1;
    }

    if (sem_init(t->empty, 0, nmemb)) {
        sem_destroy(t->fill);
        return -1;
    }

    if (sem_init(t->mutex, 0, 1)) {
        sem_destroy(t->fill);
        sem_destroy(t->empty);
        return -1;
    }

    if (!(t->buf = malloc(size * nmemb)))
    {
        sem_destroy(t->fill);
        sem_destroy(t->empty);
        sem_destroy(t->mutex);
        errno = ENOMEM;
        return -1;
    }

    t->size = size;
    t->nmemb = nmemb;
    t->pos = 0;
    t->open = true;

    return 0;
}

void tipe_destroy(tipe *t)
{
    sem_destroy(t->fill);
    sem_destroy(t->empty);
    sem_destroy(t->mutex);
    free(t->buf);
}


void tipe_open(tipe *t)
{
    t->open = true;
}

void tipe_close(tipe *t)
{
    t->open = false;
}


int tipe_write(tipe *t, void *obj)
{
    if (!t->open)
        return -1;

    sem_wait(t->empty);
    sem_wait(t->mutex);

    assert(t->pos < t->nmemb);

    memcpy(&t->buf[t->size * t->pos], obj, t->size);
    t->pos++;

    sem_post(t->mutex);
    sem_post(t->fill);

    return 0;
}

int tipe_read(tipe *t, void *obj)
{
    if (!t->open && !t->pos)
        return -1;

    sem_wait(t->fill);
    sem_wait(t->mutex);

    assert(t->pos > 0);

    t->pos--;
    memcpy(obj, &t->buf[t->size * t->pos], t->size);

    sem_post(t->mutex);
    sem_post(t->empty);

    return 0;
}
