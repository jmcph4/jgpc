#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "str.h"

unsigned int count_occurrences(char* haystack, char needle)
{
    if(haystack == NULL) /* null guard */
    {
        return 0;
    }

    if(needle == '\0') /* bounds check */
    {
        return 0;
    }

    unsigned int n = 0;

    for(unsigned int i=0;i<strlen(haystack);i++)
    {
        if(haystack[i] == needle)
        {
            n++;
        }
    }

    return n;
}

char** explode(char* s, char delim)
{
    if(s == NULL) /* null guard */
    {
        return NULL;
    }

    if(delim == '\0') /* bounds check */
    {
        return NULL;
    }

    unsigned int num_elems = count_occurrences(s, delim) + 1;
    char** elems = calloc(num_elems + 1, sizeof(char*));

    if(elems == NULL) /* allocation check */
    {
        return NULL;
    }

    unsigned int e = 0; /* number of elements captured */
    unsigned int run = 0; /* length of current run */

    /* traverse string extracting elements into elems array */
    for(unsigned int i=0;i<=strlen(s);i++)
    {
        if(s[i] == delim || s[i] == '\0') /* hit delimiter */
        {
            elems[e] = calloc(run + 1, sizeof(char));

            if(elems[e] == NULL) /* allocation check */
            {
                return NULL;
            }

            strncpy(elems[e], &s[i-run], run); /* copy string into buffer */

            e++; /* increment element counter */
            run = 0; /* reset run length counter */
        }
        else
        {
            run++; /* increment run length counter */
        }
    }

    elems[num_elems] = NULL; /* NULL-terminate elems array */

    return elems;
}

char** explode_line(char* line, char delim)
{
    if(line == NULL) /* null guard */
    {
        return NULL;
    }

    if(delim == '\0')
    {
        return NULL;
    }

    char** line_elems = explode(line, '\n');
    char** elems = explode(line_elems[0], ' ');
    
    /* free line_elems array */
    for(char** ptr=line_elems;*ptr!=NULL;ptr++)
    {
        free(*ptr);
    }
    
    free(line_elems);

    return elems;
}

char* capitalise(char* s)
{
    if(s == NULL) /* null guard */
    {
        return NULL;
    }

    if(strlen(s) == 0) /* bounds check */
    {
        return NULL;
    }

    char* capitalised = NULL;

    if(isalpha(s[0]))
    {
        /* allocate space for new string */
        capitalised = calloc(strlen(s) + 1, sizeof(char));

        if(capitalised == NULL) /* allocation check */
        {
            return NULL;
        }

        strncpy(capitalised, s, strlen(s)); /* copy into new string */

        if(islower(s[0])) /* first character is lower-case */
        {
            capitalised[0] -= ('a' - 'A'); /* ASCII shift */
        }
    }
    else /* first character is not a letter */
    {
        return NULL;
    }

    return capitalised;
}

