#include <x86intrin.h>
#include "utilities.h"
#include "hash_table.h"

int main(int argc, const char *argv[])
{
    const int CORRECT_NUMBER_ARGC = 5;

    if(!check_argc(argc, CORRECT_NUMBER_ARGC))
        return -1;

    size_t t1 = __rdtsc();

    const char  *file_name_input      =               argv[1];
    const char  *file_name_output     =               argv[2];
    const size_t size_hash_table      = (size_t) atoi(argv[3]);
    const int    hash_function_number =          atoi(argv[4]);

    MYASSERT(size_hash_table < HASH_TABLE_MAX_SIZE, EXCEEDING_MAX_SIZE, return EXCEEDING_MAX_SIZE);

    FILE *src_text_file_pointer = check_isopen(file_name_input,  "r");
    FILE *output_file_pointer   = check_isopen(file_name_output, "w");

    MYASSERT(src_text_file_pointer != NULL, COULD_NOT_OPEN_THE_FILE , return COULD_NOT_OPEN_THE_FILE);
    MYASSERT(output_file_pointer   != NULL, COULD_NOT_OPEN_THE_FILE , return COULD_NOT_OPEN_THE_FILE);

    struct text_parametrs source_text = {};

    text_parametrs_constructor(&source_text, src_text_file_pointer);

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
        case 10:
            hash_table_ctor(main_hash_table, size_hash_table, crc32_hash_sse);
            break;
        default: return 0; break;
    }

    hash_table_input_data(main_hash_table, &source_text);

    hash_table_stress_test(main_hash_table, &source_text);

    hash_table_output_data(main_hash_table,  output_file_pointer);

    hash_table_dtor(main_hash_table);
    text_parametrs_destructor(&source_text);

    size_t t2 = __rdtsc();
    size_t time = (t2 - t1);
    printf("\n>>> Time of StressTest: %lu\n", time);

    MYASSERT(check_isclose (src_text_file_pointer), COULD_NOT_CLOSE_THE_FILE , return COULD_NOT_CLOSE_THE_FILE);
    MYASSERT(check_isclose (output_file_pointer),   COULD_NOT_CLOSE_THE_FILE , return COULD_NOT_CLOSE_THE_FILE);

    return 0;
}

