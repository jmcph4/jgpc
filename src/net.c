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
        int (callback)(void*, size_t, void**, struct clientinfo*))
{
    if(server_info == NULL || callback == NULL) /* null guard */
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

        res = setsockopt(server_info->socket_info.fd, SOL_SOCKET, SO_REUSEADDR,
                    &reuse_val, sizeof(int));
        
        if(res == -1)
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

    /* if TCP */
    if(server_info->socket_info.type == SOCK_STREAM)
    {
        /* listen on socket */
        res = listen(server_info->socket_info.fd, SOMAXCONN);

        if(res == -1)
        {
            return -1;
        }
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
        
        num_conns++; /* increment connection counter */

        struct clientinfo client_info;
        client_info.addr = from_addr.sin_addr.s_addr;

        /* get hostname of client */
        client_info.hostname = calloc(128, sizeof(char));

        if(client_info.hostname == NULL) /* allocation check */
        {
            return -1;
        }

        res = getnameinfo((struct sockaddr*)&from_addr, from_addr_size,
                client_info.hostname, 128, NULL, 0, 0);

        if(res != 0)
        {
            free(client_info.hostname);
            return -1;
        }

        pid_t pid = fork();

        if(pid == 0)
        {
            char* resp_buf = NULL;
            int resp_buf_len = 0;
        
            while(true)
            {
                char* client_data = calloc(server_info->max_buf_len,
                        sizeof(char));

                if(client_data == NULL)
                {
                    break;
                }

                /* get data from client */
                res = recv(fd, client_data, server_info->max_buf_len, 0);

                if(res == -1 || res == 0)
                {
                    free(client_data);
                    break;
                }
 
                resp_buf_len = callback(client_data, res, (void**)&resp_buf,
                        &client_info);

                if(resp_buf_len == -1)
                {
                    free(client_data);
                    break;
                }

                /* send output of worker back to client */
                res = send(fd, resp_buf, resp_buf_len, 0);

                if(res == -1)
                {
                    free(client_data);
                    free(resp_buf);
                    break;
                }

                free(client_data);
                free(resp_buf);
            }

            free(client_info.hostname);

            close(fd);
            exit(0);
        }
        else if(pid > 0)
        {
            free(client_info.hostname);
        }
        else
        {
            return -1;
        }
    }

    return num_conns;
}

