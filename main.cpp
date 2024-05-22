#include <string.h>
#include <emmintrin.h>
#include <x86intrin.h>
#include "libraries/Stack/myassert.h"
#include "libraries/Stack/stack.h"
#include "libraries/Onegin/onegin.h"
#include "hash_table.h"

const size_t AMOUNT_TESTS = 512;

errors_code insert_data(hash_table *table, text_parametrs *source_text);
errors_code write_data (hash_table *table, FILE *file_output);
errors_code stress_test(hash_table *table, text_parametrs *source_text);

int main(int argc, const char *argv[])
{
    const int CORRECT_NUMBER_ARGC = 5;

    if(!check_argc(argc, CORRECT_NUMBER_ARGC))
        return -1;

    size_t t1 = __rdtsc();

    const char  *file_name_input      = argv[1];
    const char  *file_name_output     = argv[2];
    const size_t size_hash_table      = (size_t) atoi(argv[3]);
    const int    hash_function_number = atoi(argv[4]);

    MYASSERT(size_hash_table < HASH_table_MAX_SIZE, EXCEEDING_MAX_SIZE, return EXCEEDING_MAX_SIZE);

    FILE *src_text_file_pointer = check_isopen_old(file_name_input,  "r");
    FILE *output_file_pointer   = check_isopen_old(file_name_output, "w");

    MYASSERT(src_text_file_pointer != NULL, COULD_NOT_OPEN_THE_FILE , return COULD_NOT_OPEN_THE_FILE);
    MYASSERT(output_file_pointer   != NULL, COULD_NOT_OPEN_THE_FILE , return COULD_NOT_OPEN_THE_FILE);

    struct text_parametrs source_text = {};

    text_parametrs_constructor(&source_text, src_text_file_pointer);
    replace_chars_to_null_character(source_text.buffer, "\n");
    source_text.number_lines--;

    hash_table *main_hash_table = new_pointer_hash_table();

    switch (hash_function_number)
    {
        case 1:
            hash_table_ctor(main_hash_table, size_hash_table, zero_hash);
            break;
        case 2:
            hash_table_ctor(main_hash_table, size_hash_table, first_letter_hash);
            break;
        case 3:
            hash_table_ctor(main_hash_table, size_hash_table, length_hash);
            break;
        case 4:
            hash_table_ctor(main_hash_table, size_hash_table, sum_hash);
            break;
        case 5:
            hash_table_ctor(main_hash_table, size_hash_table, sum_length_hash);
            break;
        case 6:
            hash_table_ctor(main_hash_table, size_hash_table, ror_hash);
            break;
        case 7:
            hash_table_ctor(main_hash_table, size_hash_table, rol_hash);
            break;
        case 8:
            hash_table_ctor(main_hash_table, size_hash_table, elf_hash);
            break;
        case 9:
            hash_table_ctor(main_hash_table, size_hash_table, crc32_hash);
            break;
        default: return 0; break;
    }

    insert_data(main_hash_table, &source_text);
    write_data(main_hash_table,  output_file_pointer);

    hash_table_dtor(main_hash_table);
    text_parametrs_destructor(&source_text);

    size_t t2 = __rdtsc();
    size_t time = (t2 - t1);
    printf("\n>>> Time of StressTest: %lu\n", time);

    MYASSERT(check_isclose (src_text_file_pointer), COULD_NOT_CLOSE_THE_FILE , return COULD_NOT_CLOSE_THE_FILE);
    MYASSERT(check_isclose (output_file_pointer),   COULD_NOT_CLOSE_THE_FILE , return COULD_NOT_CLOSE_THE_FILE);

    return 0;
}

errors_code insert_data(hash_table *table, text_parametrs *source_text)
{
    MYASSERT(table                      != NULL, NULL_POINTER_PASSED_TO_FUNC, return NULL_POINTER_PASSED_TO_FUNC);
    MYASSERT(table->hash_function       != NULL, NULL_POINTER_PASSED_TO_FUNC, return NULL_POINTER_PASSED_TO_FUNC);
    MYASSERT(source_text                != NULL, NULL_POINTER_PASSED_TO_FUNC, return NULL_POINTER_PASSED_TO_FUNC);
    MYASSERT(source_text->buffer        != NULL, NULL_POINTER_PASSED_TO_FUNC, return NULL_POINTER_PASSED_TO_FUNC);
    MYASSERT(source_text->string_array  != NULL, NULL_POINTER_PASSED_TO_FUNC, return NULL_POINTER_PASSED_TO_FUNC);

    for (size_t i = 0; i < (source_text->number_lines); i++)
    {
        hash_table_add(table, source_text->string_array[i].string_pointer, source_text->string_array[i].size_string - 1);
    }

    return ASSERT_NO_ERROR;
}

errors_code write_data(hash_table *table, FILE *file_output)
{
    MYASSERT(table                      != NULL, NULL_POINTER_PASSED_TO_FUNC, return NULL_POINTER_PASSED_TO_FUNC);
    MYASSERT(table->hash_function       != NULL, NULL_POINTER_PASSED_TO_FUNC, return NULL_POINTER_PASSED_TO_FUNC);
    MYASSERT(file_output                != NULL, NULL_POINTER_PASSED_TO_FUNC, return NULL_POINTER_PASSED_TO_FUNC);

    for (size_t i = 0; i < (table->size); i++)
    {
        size_t cases = 0;
        ssize_t index_element_list = table->items[i].list_pointer->next[0];

        while (index_element_list != 0)
        {
            cases++;
            index_element_list = table->items[i].list_pointer->next[index_element_list];
        }

        fprintf(file_output, "%lu\n", cases);
    }

    return ASSERT_NO_ERROR;
}

errors_code stress_test(hash_table *table, text_parametrs *source_text)
{
    MYASSERT(table                      != NULL, NULL_POINTER_PASSED_TO_FUNC, return NULL_POINTER_PASSED_TO_FUNC);
    MYASSERT(table->hash_function       != NULL, NULL_POINTER_PASSED_TO_FUNC, return NULL_POINTER_PASSED_TO_FUNC);
    MYASSERT(source_text                != NULL, NULL_POINTER_PASSED_TO_FUNC, return NULL_POINTER_PASSED_TO_FUNC);
    MYASSERT(source_text->buffer        != NULL, NULL_POINTER_PASSED_TO_FUNC, return NULL_POINTER_PASSED_TO_FUNC);
    MYASSERT(source_text->string_array  != NULL, NULL_POINTER_PASSED_TO_FUNC, return NULL_POINTER_PASSED_TO_FUNC);

    char *unknown_word = "qwerty";

    for (size_t i = 0; i < AMOUNT_TESTS; i++)
    {
        volatile TYPE_ELEMENT_LIST find_elem = hash_table_search(table, unknown_word, 6);

        for (size_t j = 0; j < (source_text->number_lines); j++)
        {
            volatile TYPE_ELEMENT_LIST find_elem = hash_table_search(table, source_text->string_array[i].string_pointer, source_text->string_array[i].size_string - 1);
        }
    }

    return ASSERT_NO_ERROR;
}

