#ifndef PROC_H_
#define PROC_H_

#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <fcntl.h>

struct procinfo
{
    pid_t pid;
    char* prog;
    int argc;
    char** argv;
    FILE* stdin;
    FILE* stdout;
    FILE* stderr;
};

int process_spawn(struct procinfo* process_info);
int process_stop(struct procinfo* process_info);

#endif /* PROC_H_ */

