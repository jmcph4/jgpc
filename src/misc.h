#ifndef MISC_H_
#define MISC_H_

#define COLUMN_BORDER_CHAR '|'
#define HEADER_BORDER_CHAR '='
#define BORDER_CHAR '-'
#define JOIN_CHAR '+'
#define PADDING_CHAR ' '

enum justify_type
{
    LEFT_JUSTIFY,
    RIGHT_JUSTIFY,
    CENTRE_JUSTIFY
};

unsigned int uint_max(unsigned int* arr, unsigned int len);
unsigned int uint_sum(unsigned int* arr, unsigned int len);

char* justify(char* s, unsigned int len, enum justify_type type);
void print_table(enum justify_type justify_mode, unsigned int rows,
        unsigned int cols, char*** table);

#endif /* MISC_H_ */

