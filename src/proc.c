#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fcntl.h> 
#include <unistd.h>
#include <sys/wait.h>

#include "proc.h"

int spawn(struct procinfo* process_info)
{
    if(process_info == NULL) /* null guard */
    {
        return -1;
    }

    /* aliasing */
    char* prog = process_info->prog;
    int argc = process_info->argc;
    char** argv = process_info->argv;

    if(argc == 0) /* bounds check */
    {
        return -1;
    }

    /* create pipes */
    int pipe_res = 0;
    int to_fds[2] = {0, 0};
    int from_fds[2] = {0, 0};
    int err_fds[2] = {0, 0};

    pipe_res = pipe(to_fds);

    if(pipe_res == -1)
    {
        return -1;
    }

    pipe_res = pipe(from_fds);

    if(pipe_res == -1)
    {
        return -1;
    }

    pipe_res = pipe(err_fds);

    if(pipe_res == -1)
    {
        return -1;
    }

    pid_t pid = fork();

    if(pid == 0) /* child */
    {
        /* set up pipes */
        dup2(to_fds[0], STDIN_FILENO); /* connect to stdin */
        dup2(from_fds[1], STDOUT_FILENO); /* connect to stdout */
        dup2(err_fds[1], STDERR_FILENO); /* connect to stderr */
        
        /* copy argument list into appropriate format for exec family */
        char** args = calloc(argc, sizeof(char*));
        
        if(args == NULL)
        {
            exit(EXIT_FAILURE);
        }

        for(unsigned int i=0;(int)i<argc;i++)
        {
            if(i == 0)
            {
                args[i] = calloc(strlen(prog) + 1, sizeof(char));

                if(args[i] == NULL)
                {
                    exit(EXIT_FAILURE);
                }

                strncpy(args[i], prog, strlen(prog));
                continue;
            }

            args[i] = calloc(strlen(argv[i-1]) + 1, sizeof(char));
            
            if(args[i] == NULL) /* allocation check */
            {
                exit(EXIT_FAILURE);
            }

            strncpy(args[i], argv[i-1], strlen(argv[i-1]));
        }

        args[argc] = NULL; /* NULL-terminate for exec family */

        int res = execv(prog, args); /* execute program */
        
        if(res == -1)
        {
            exit(EXIT_FAILURE);
        }
    }
    else if(pid > 0) /* parent */
    {
        close(to_fds[0]); /* close read end */
        close(from_fds[1]); /* close write end */
        close(err_fds[1]); /* close write end */

        FILE* to = fdopen(to_fds[1], "w");

        if(to == NULL)
        {
            return -1;
        }

        FILE* from = fdopen(from_fds[0], "r");

        if(from == NULL)
        {
            return -1;
        }

        FILE* err = fdopen(err_fds[0], "r");

        if(err == NULL)
        {
            return -1;
        }

        process_info->stdin = to;
        process_info->stdout = from;
        process_info->stderr = err;

        process_info->pid = pid;
    }
    else /* error */
    {
        return -1;
    }

    return 0;
}

int stop(struct procinfo* process_info)
{
    if(process_info == NULL) /* null guard */
    {
        return -1;
    }
    
    int status = 0;
    int options = 0;

    int res = waitpid(process_info->pid, &status, options);

    if(res == -1)
    {
        return -1;
    }

    return status;
}

