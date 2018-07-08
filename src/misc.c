#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "misc.h"

unsigned int uint_max(unsigned int* arr, unsigned int len)
{
    if(arr == NULL) /* null guard */
    {
        return 0;
    }

    if(len == 0) /* bounds check */
    {
        return 0;
    }

    unsigned int max = 0;

    for(unsigned int i=0;i<len;i++)
    {
        if(i == 0)
        {
            max = arr[i];
        }
        else
        {
            if(arr[i] > max)
            {
                max = arr[i];
            }
        }
    }

    return max;
}

unsigned int uint_sum(unsigned int* arr, unsigned int len)
{
    if(arr == NULL) /* null guard */
    {
        return 0;
    }

    if(len == 0) /* bounds check */
    {
        return 0;
    }

    unsigned int sum = 0;

    for(unsigned int i=0;i<len;i++)
    {
        sum += arr[i];
    }

    return sum;
}

unsigned int* calculate_column_extents(unsigned int rows, unsigned int cols,
        char*** table)
{
    if(table == NULL) /* null guard */
    {
        return NULL;
    }

    if(rows == 0 || cols == 0) /* bounds check */
    {
        return NULL;
    }

    unsigned int** col_widths = calloc(cols, sizeof(unsigned int*));

    if(col_widths == NULL)
    {
        return NULL;
    }

    for(unsigned int i=0;i<cols;i++)
    {
        col_widths[i] = calloc(rows, sizeof(unsigned int));
    }

    for(unsigned int i=0;i<rows;i++)
    {
        for(unsigned int j=0;j<cols;j++)
        {
            col_widths[j][i] = strlen(table[i][j]);
        }
    }

    unsigned int* column_extents = calloc(cols, sizeof(unsigned int));

    if(column_extents == NULL) /* allocation check */
    {
        return NULL;
    }

    for(unsigned int i=0;i<cols;i++)
    {
        column_extents[i] = uint_max(col_widths[i], rows);
    }

    for(unsigned int i=0;i<cols;i++)
    {
        free(col_widths[i]);
    }

    free(col_widths);

    return column_extents;
}

char* generate_border(unsigned int* extents, unsigned int len, bool header)
{
    if(extents == NULL) /* bounds check */
    {
        return NULL;
    }

    if(len == 0) /* bounds check */
    {
        return NULL;
    }

    unsigned int total_width = uint_sum(extents, len) + len + 2;

    char* border = calloc(total_width, sizeof(char));

    if(border == NULL) /* check for failure */
    {
        return NULL;
    }

    unsigned int col = 0; /* which column we're up to */
    unsigned int distance = 0; /* how many chars until end of current column */

    for(unsigned int i=0;i<total_width;i++)
    {
        if(distance == 0)
        {
            border[i] = JOIN_CHAR;
            
            if(i > 0)
            {
                col++;
            }

            distance = extents[col];
        }
        else
        {
            if(header)
            {
                border[i] = HEADER_BORDER_CHAR;
            }
            else
            {
                border[i] = BORDER_CHAR;
            }
            distance--;
        }
    }

    border[total_width-1] = '\0';

    return border;
}

char* left_justify(char* s, unsigned int len)
{
    if(s == NULL) /* null guard */
    {
        return NULL;
    }

    if(len == 0 || strlen(s) > len) /* bounds check */
    {
        return NULL;
    }

    char* justified = calloc(len + 1, sizeof(char));

    if(justified == NULL) /* allocation check */
    {
        return NULL;
    }

    unsigned int diff = len - strlen(s);

    if(diff == 0)
    {
        strncpy(justified, s, strlen(s));
    }
    else
    {
        strncpy(justified, s, strlen(s));

        for(unsigned int i=0;i<diff;i++)
        {
            justified[strlen(s)+i] = PADDING_CHAR;
        }
    }

    return justified;
}

char* right_justify(char* s, unsigned int len)
{
    if(s == NULL) /* null guard */
    {
        return NULL;
    }

    if(len == 0 || strlen(s) > len) /* bounds check */
    {
        return NULL;
    }

    char* justified = calloc(len + 1, sizeof(char));

    if(justified == NULL) /* allocation check */
    {
        return NULL;
    }

    unsigned int diff = len - strlen(s);

    if(diff == 0)
    {
        strncpy(justified, s, strlen(s));
    }
    else
    {
        for(unsigned int i=0;i<diff;i++)
        {
            justified[i] = PADDING_CHAR;
        }
    }

    strncpy(&justified[diff], s, strlen(s));

    return justified;
}

char* centre_justify(char* s, unsigned int len)
{
    if(s == NULL) /* null guard */
    {
        return NULL;
    }

    if(len == 0 || strlen(s) > len) /* bounds check */
    {
        return NULL;
    }

    char* justified = calloc(len + 1, sizeof(char));

    if(justified == NULL) /* allocation check */
    {
        return NULL;
    }

    unsigned int diff = len - strlen(s);

    if(diff == 0)
    {
        strncpy(justified, s, strlen(s));
    }
    else
    {
        for(unsigned int i=0;i<diff/2;i++)
        {
            justified[i] = PADDING_CHAR;
        }

        strncpy(&justified[diff/2], s, strlen(s));

        for(unsigned int i=0;i<diff/2;i++)
        {
            justified[diff/2+strlen(s)+i] = PADDING_CHAR;
        }

        /* handle the case where the difference is odd */
        if(diff % 2 != 0)
        {
            justified[len-1] = PADDING_CHAR;
        }
    }

    return justified;
}

char* justify(char* s, unsigned int len, enum justify_type type)
{
    switch(type)
    {
        case LEFT_JUSTIFY:
            return left_justify(s, len);
            break;
        case RIGHT_JUSTIFY:
            return right_justify(s, len);
            break;
        case CENTRE_JUSTIFY:
            return centre_justify(s, len);
            break;
        default:
            return NULL;
            break;
    }
}

void print_table(enum justify_type justify_mode, unsigned int rows,
        unsigned int cols, char*** table)
{
    if(table == NULL) /* null guard */
    {
        return;
    }

    if(rows == 0 || cols == 0) /* bounds check */
    {
        return;
    }

    unsigned int* column_extents = calculate_column_extents(rows, cols, table);

    if(column_extents == NULL)
    {
        return;
    }

    /* top border */
    char* border = generate_border(column_extents, cols, false);
    
    if(border == NULL)
    {
        return;
    }

    printf("%s\n", border);
    free(border);

    for(unsigned int i=0;i<rows;i++)
    {
        printf("%c", COLUMN_BORDER_CHAR);

        for(unsigned int j=0;j<cols;j++)
        {
            char* justified_text = justify(table[i][j], column_extents[j],
                    justify_mode);

            if(justified_text == NULL)
            {
                return;
            }

            printf("%s%c", justified_text, COLUMN_BORDER_CHAR);

            free(justified_text);
        }

        printf("\n");

        /* handle header row */
        if(i == 0)
        {
            border = generate_border(column_extents, cols, true);
            
            if(border == NULL)
            {
                return;
            }
            
            printf(border);
            printf("\n");
        }
    }

    /* bottom border */
    border = generate_border(column_extents, cols, false);
    
    if(border == NULL)
    {
        return;
    }
    
    printf(border);
    printf("\n");

    free(column_extents);
}

