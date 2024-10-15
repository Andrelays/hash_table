#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "onegin.h"
#include "myassert.h"
#include "list.h"
#include "hash_table.h"

static char *move_str_aligned_area(size_t alignment, size_t size, char *str);

void text_parametrs_constructor(struct text_parametrs *text, FILE *file_pointer)
{

    MYASSERT(text != NULL, NULL_POINTER_PASSED_TO_FUNC, return);

    text->buffer = input_data(file_pointer);

    search_strings(text);

    for(size_t index = 0; index < text->number_lines - 1; index++) {
        ((text->string_array)[index]).string_pointer = move_str_aligned_area(ALIGNMENT_COEF, ((text->string_array)[index]).size_string,
                                                                                             ((text->string_array)[index]).string_pointer);
    }
}

char *input_data(FILE *file_pointer)
{
    size_t size_file = determine_size(file_pointer);

    char *buffer = (char *)calloc(size_file + 1, sizeof(char));
    MYASSERT(buffer != NULL, NULL_POINTER_PASSED_TO_FUNC, return NULL);

    size_file = fread(buffer, sizeof(char), size_file, file_pointer);

    buffer[size_file] = '\0';

    return buffer;
}

size_t determine_size(FILE *file_pointer)
{
    MYASSERT(file_pointer != NULL, NULL_POINTER_PASSED_TO_FUNC, return 0);

    fseek(file_pointer, 0, SEEK_END);
    long size = ftell(file_pointer);

    MYASSERT(size >= 0, NEGATIVE_VALUE_SIZE_T, return 0);

    rewind(file_pointer);

    return (size_t) size;
}

void search_strings(struct text_parametrs *text)
{
    MYASSERT(text         != NULL, NULL_POINTER_PASSED_TO_FUNC, return);
    MYASSERT(text->buffer != NULL, NULL_POINTER_PASSED_TO_FUNC, return);

    text->number_lines = count_strings(text->buffer);

    text->string_array = (string_parametrs *) calloc(text->number_lines, sizeof(string_parametrs));

    MYASSERT(text->string_array != NULL, NULL_POINTER_PASSED_TO_FUNC, return);

    size_t index = 0;
    char *string_pointer = text->buffer;

    for (index = 0; index < text->number_lines - 1; index++)
    {
        ((text->string_array)[index]).string_pointer = string_pointer;

        string_pointer = strchr(string_pointer, '\n') + 1;

        ((text->string_array)[index]).size_string = (size_t) (string_pointer - ((text->string_array)[index]).string_pointer);
    }

    ((text->string_array)[index]).string_pointer = string_pointer;
    ((text->string_array)[index]).size_string = (size_t) (strchr(string_pointer, '\0') - ((text->string_array)[index]).string_pointer + 1);
}

size_t count_strings(const char *buffer)
{
    MYASSERT(buffer != NULL, NULL_POINTER_PASSED_TO_FUNC, return 0);

    size_t count = 0;

    for (size_t index = 0; buffer[index] != '\0'; index++) {
        if (buffer[index] == '\n') {
            count++;
        }
    }

    return (count);
}

void text_parametrs_destructor(struct text_parametrs *text)
{
    MYASSERT(text != NULL, NULL_POINTER_PASSED_TO_FUNC, return);

    free(text->buffer);

    for(size_t index = 0; index < text->number_lines - 1; index++) {
        memset((text->string_array)[index].string_pointer, 0, ALIGNMENT_COEF);
        free(((text->string_array)[index]).string_pointer);
    }

    free(text->string_array);

    text->number_lines = 0;
}


static char *move_str_aligned_area(size_t alignment, size_t size, char *str)
{
    MYASSERT(str != NULL,       NULL_POINTER_PASSED_TO_FUNC, return NULL);
    MYASSERT(alignment >= size, EXCEEDING_MAX_SIZE,          return NULL);

    char *aligned_str = (char *) aligned_alloc(alignment, alignment);
    memset(aligned_str, 0, alignment);

    strncpy(aligned_str, str, size);

    return aligned_str;
}
