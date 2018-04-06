#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include "input.h"

int grabeof(char** buf, FILE* fp)
{
    if(buf == NULL || fp == NULL) /* null guard */
    {
        return -1;
    }

    char c = '\0';
    size_t len = 0;
    size_t cap = INIT_BUF_LEN;
    char* buffer = calloc(cap, sizeof(char));

    if(buffer == NULL) /* allocation check */
    {
        return -1;
    }

    while(true)
    {
        c = fgetc(fp);

        if(c == EOF) /* handle EOF */
        {
            break;
        }

        if(len + 1 == cap) /* expand buffer */
        {
            cap *= BUF_GROWTH_FACTOR;
            buffer = realloc(buffer, cap);
        }

        buffer[len] = c;
        len++;
    }

    *buf = buffer;

    return len;
}

int grabuntil(char** buf, bool* eof, char* delims, size_t num_delims, FILE* fp)
{
    /* null guard */
    if(buf == NULL || eof == NULL || delims == NULL || fp == NULL)
    {
        return -1;
    }

    /* allocate our internal buffer */
    size_t cap = INIT_BUF_LEN;
    char* buffer = calloc(cap, sizeof(char));

    if(buffer == NULL) /* allocation check */
    {
        return -1;
    }

    bool hit_delim = false; /* whether c is a delimiter or not */
    unsigned int n = 0; /* number of characters stored */
    char c = '\0'; /* current character */

    do /* main loop */
    {
        c = fgetc(fp);

        if(c == EOF)
        {
            *eof = true;
            break;
        }
        
        /* check for delimiter */
        for(unsigned int i=0;i<num_delims;i++)
        {
            if(c == delims[i])
            {
                hit_delim = true;
                break;
            }
        }

        if(hit_delim)
        {
            break;
        }

        /* store in buffer */
        buffer[n] = c;
        n++;

        if(n + 1 == cap) /* expand internal buffer if needed */
        {
            cap *= BUF_GROWTH_FACTOR;
            buffer = realloc(buffer, cap);
        }
    }
    while(c != EOF);

    if(n == 0)
    {
        free(buffer);
    }
    else
    {
        buffer[n] = '\0'; /* null terminate */
        *buf = buffer; /* write out */
    }
    
    return (int)n;
}

int grabline(char** buf, bool* eof, FILE* fp)
{
    if(buf == NULL || eof == NULL || fp == NULL) /* null guard */
    {
        return -1;
    }

    char delimiter = '\n';
    return grabuntil(buf, eof, &delimiter, 1, fp);
}

int readfile(const char* path, char** data, size_t* size)
{
    if(path == NULL || data == NULL || size == NULL) /* null guard */
    {
        return -1;
    }

    FILE* file = fopen(path, "rb");

    if(file == NULL)
    {
        return -1;
    }

    /* determine file size */
    fseek(file, 0, SEEK_END);
    *size = ftell(file);
    rewind(file);
    
    *data = calloc(*size, sizeof(char));

    if(*data == NULL) /* allocation check */
    {
        return -1;
    }

    size_t bytes_read  = fread(*data, sizeof(uint8_t), *size, file);

    if(bytes_read < *size)
    {
        free(*data);
        return -1;
    }

    fclose(file);

    return 0;
}

