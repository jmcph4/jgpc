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

int spawn(struct procinfo* process_info);
int stop(struct procinfo* process_info);

#endif /* PROC_H_ */

