#ifndef THREAD_H_
#define THREAD_H_

#include <stdbool.h>

#include <pthread.h>

struct threadinfo
{
    pthread_t tid;          /* thread ID */
    pthread_attr_t attr;    /* thread attributes */
    void* (*fn)(void*);     /* routine thread will enter */
    void* fn_arg;           /* sole argument to routine */
    void* fn_retval;        /* return value of routine */
    bool detach;            /* make thread detachable or not */
};

int thread_spawn(struct threadinfo* thread_info);
int thread_stop(struct threadinfo* thread_info);

#endif /* THREAD_H_ */

