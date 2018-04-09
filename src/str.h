#ifndef STR_H_
#define STR_H_

#define STRINGS_EQUAL(a,b) (strcmp(a,b) == 0)

unsigned int count_occurrences(char* haystack, char needle);
char** explode(char* s, char delim);
char** explode_line(char* line, char delim);
char* capitalise(char* s);

#endif /* STR_H_ */

