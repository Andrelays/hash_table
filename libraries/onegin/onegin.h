#ifndef ONEGIN_H_INCLUDED
#define ONEGIN_H_INCLUDED

#include <stdio.h>

struct text_parametrs {
    struct string_parametrs *string_array;
    char                    *buffer;
    size_t                   number_lines;
};

struct string_parametrs {
    char            *string_pointer;
    size_t           size_string;
};

void   text_parametrs_constructor(struct text_parametrs *, FILE *);
char  *input_data                (FILE *);
size_t determine_size            (FILE *);
void   search_strings            (struct text_parametrs *);
size_t count_strings             (const char *);
void   text_parametrs_destructor (struct text_parametrs *);
#endif // ONEGIN_H_INCLUDED
