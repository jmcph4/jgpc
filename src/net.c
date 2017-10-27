#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

#include "input.h"
#include "net.h"

int startserver_mp(struct serverinfo* server_info,
        struct procinfo* process_info)
{
    if(server_info == NULL || process_info == NULL) /* null guard */
    {
        return -1;
    }

    int res = 0;

    struct sockaddr_in server_addr;

    server_info->socket_info.fd = socket(server_info->socket_info.domain,
            server_info->socket_info.type, server_info->socket_info.protocol);

    if(server_info->socket_info.fd == -1)
    {
        return -1;
    }

    /* reuse socket if specified */
    if(server_info->socket_info.reuse)
    {
        int reuse_val = 1;

        if(setsockopt(server_info->socket_info.fd, SOL_SOCKET, SO_REUSEADDR,
                    &reuse_val, sizeof(int)) == -1)
        {
            return -1;
        }
    }

    /* set up server_addr struct */
    server_addr.sin_family = server_info->socket_info.domain;
    server_addr.sin_port = htons(server_info->port);
    server_addr.sin_addr.s_addr = htonl(server_info->addr);

    /* bind to port */
    res = bind(server_info->socket_info.fd, (struct sockaddr*)&server_addr,
            sizeof(struct sockaddr_in));

    if(res == -1)
    {
        return -1;
    }

    /* listen on socket */
    res = listen(server_info->socket_info.fd, SOMAXCONN);

    if(res == -1)
    {
        return -1;
    }

    int num_conns = 0; /* connec */

    /* main service loop */
    while(true)
    {
        /* accept connection */
        struct sockaddr_in from_addr;
        socklen_t from_addr_size = sizeof(struct sockaddr_in);

        int fd = accept(server_info->socket_info.fd,
                (struct sockaddr*)&from_addr, &from_addr_size);

        if(fd == -1)
        {
            return -1;
        }

        FILE* client_fp = fdopen(fd, "rw");

        num_conns++; /* increment connection counter */

        /* get hostname of client */
        char hostname[128];
        res = getnameinfo((struct sockaddr*)&from_addr, from_addr_size,
                hostname, 128, NULL, 0, 0);

        if(res != 0)
        {
            return -1;
        }
        
        /* DEBUG */
        printf("Accepted connection from %s (%s) on port %d\n",
                inet_ntoa(from_addr.sin_addr), hostname,
                ntohs(from_addr.sin_port));

        res = spawn(process_info);

        if(res == -1)
        {
            return -1;
        }

        char* resp_buf = NULL;
        bool worker_eof = false;

        res = grabline(&resp_buf, &worker_eof, process_info->stdout);

        if(res == -1)
        {
            return -1;
        }

        res = fwrite(resp_buf, sizeof(char), strlen(resp_buf), client_fp);

        if(res == -1)
        {
            return -1;
        }

        int worker_status = stop(process_info); /* wind down worker */

        if(worker_status == -1)
        {
            return -1;
        }
    }

    return num_conns;
}

