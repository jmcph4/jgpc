#include <stdlib.h>

#include <pthread.h>

#include "thread.h"

int thread_spawn(struct threadinfo* thread_info)
{
    if(thread_info == NULL) /* null guard */
    {
        return -1;
    }

    int res = pthread_create(&thread_info->tid, &thread_info->attr,
                                thread_info->fn, thread_info->fn_arg);

    if(res != 0)
    {
        return -1;
    }

    return 0;
}

int thread_stop(struct threadinfo* thread_info)
{
    if(thread_info == NULL) /* null guard */
    {
        return -1;
    }

    int res = pthread_join(thread_info->tid, &thread_info->fn_retval);

    if(res != 0)
    {
        return -1;
    }

    return 0;
}

