#include <stdio.h>
#include <string.h>
#include <immintrin.h>
#include "hash_table.h"

static inline __attribute__((always_inline)) ssize_t find_elem_by_value(list *list_pointer, TYPE_ELEMENT_LIST value);
static inline __attribute__((always_inline)) bool check_equality_key_avx(TYPE_ELEMENT_LIST, TYPE_ELEMENT_LIST);

errors_code hash_table_ctor(hash_table *table, size_t size, size_t (*hash_function) (ht_key_t, ht_value_t))
{
    MYASSERT(table         != NULL, NULL_POINTER_PASSED_TO_FUNC, return NULL_POINTER_PASSED_TO_FUNC);
    MYASSERT(hash_function != NULL, NULL_POINTER_PASSED_TO_FUNC, return NULL_POINTER_PASSED_TO_FUNC);

    table->size          = size;
    table->hash_function = hash_function;
    table->items         = (hash_table_item *) calloc(table->size, sizeof(hash_table_item));

    for (size_t i = 0; i < table->size; i++)
    {
        hash_table_init_item((table->items)+i);
    }

    return ASSERT_NO_ERROR;
}

errors_code hash_table_input_data(hash_table *table, text_parametrs *source_text)
{
    MYASSERT(table                      != NULL, NULL_POINTER_PASSED_TO_FUNC, return NULL_POINTER_PASSED_TO_FUNC);
    MYASSERT(table->hash_function       != NULL, NULL_POINTER_PASSED_TO_FUNC, return NULL_POINTER_PASSED_TO_FUNC);
    MYASSERT(source_text                != NULL, NULL_POINTER_PASSED_TO_FUNC, return NULL_POINTER_PASSED_TO_FUNC);
    MYASSERT(source_text->buffer        != NULL, NULL_POINTER_PASSED_TO_FUNC, return NULL_POINTER_PASSED_TO_FUNC);
    MYASSERT(source_text->string_array  != NULL, NULL_POINTER_PASSED_TO_FUNC, return NULL_POINTER_PASSED_TO_FUNC);

    for (size_t i = 0; i < (source_text->number_lines) - 1; i++) {
        hash_table_add(table, source_text->string_array[i].string_pointer, source_text->string_array[i].size_string - 1);
    }

    return ASSERT_NO_ERROR;
}

errors_code hash_table_init_item(hash_table_item* item)
{
    item->amount = 0;

    item->list_pointer = new_pointer_list();
    LIST_CONSTRUCTOR(item->list_pointer);

    return ASSERT_NO_ERROR;
}

hash_table *new_pointer_hash_table()
{
    struct hash_table *table = (hash_table *) calloc(1, sizeof(hash_table));

    MYASSERT(table != NULL, FAILED_TO_ALLOCATE_DYNAM_MEMOR, return NULL);

    return table;
}

errors_code hash_table_add(hash_table *table, ht_key_t key, ht_value_t value)
{
    size_t hash  = table->hash_function(key, value);
    size_t index = hash % table->size;

    if (!find_elem_by_value(table->items[index].list_pointer, key))
    {
        (table->items[index].amount)++;
        push_front(table->items[index].list_pointer, key);
    }

    return ASSERT_NO_ERROR;
}

TYPE_ELEMENT_LIST hash_table_search(hash_table *table, ht_key_t key, ht_value_t value)
{
    size_t hash  = table->hash_function(key, value);
    size_t index = hash % table->size;

    ssize_t find_node_index = find_elem_by_value(table->items[index].list_pointer, key);

    if (!find_node_index)
        return NULL;

    return table->items[index].list_pointer->data[find_node_index];
}

static inline __attribute__ ((always_inline)) ssize_t find_elem_by_value(list *list_pointer, TYPE_ELEMENT_LIST value)
{
    MYASSERT(list_pointer          != NULL, NULL_POINTER_PASSED_TO_FUNC, return 0);
    MYASSERT(list_pointer->data    != NULL, NULL_POINTER_PASSED_TO_FUNC, return 0);
    MYASSERT(list_pointer->next    != NULL, NULL_POINTER_PASSED_TO_FUNC, return 0);
    MYASSERT(list_pointer->prev    != NULL, NULL_POINTER_PASSED_TO_FUNC, return 0);
    MYASSERT(list_pointer->info    != NULL, NULL_POINTER_PASSED_TO_FUNC, return 0);

    ssize_t index_element_list = list_pointer->next[0];

    while((index_element_list != 0) && (check_equality_key_avx(value, list_pointer->data[index_element_list]))) {
        index_element_list = list_pointer->next[index_element_list];
    }

    return index_element_list;
}

static inline __attribute__((always_inline)) bool check_equality_key_avx(TYPE_ELEMENT_LIST value_1, TYPE_ELEMENT_LIST value_2)
{
    MYASSERT(value_1 != NULL, NULL_POINTER_PASSED_TO_FUNC, return POINTER_TO_LIST_IS_NULL);
    MYASSERT(value_2 != NULL, NULL_POINTER_PASSED_TO_FUNC, return POINTER_TO_LIST_IS_NULL);

    __m256i str_1 = _mm256_load_si256((const __m256i *) (value_1));
    __m256i str_2 = _mm256_load_si256((const __m256i *) (value_2));

    return (_mm256_testz_si256(str_1, str_2));
}

errors_code hash_table_stress_test(hash_table *table, text_parametrs *source_text)
{
    MYASSERT(table                      != NULL, NULL_POINTER_PASSED_TO_FUNC, return NULL_POINTER_PASSED_TO_FUNC);
    MYASSERT(table->hash_function       != NULL, NULL_POINTER_PASSED_TO_FUNC, return NULL_POINTER_PASSED_TO_FUNC);
    MYASSERT(source_text                != NULL, NULL_POINTER_PASSED_TO_FUNC, return NULL_POINTER_PASSED_TO_FUNC);
    MYASSERT(source_text->buffer        != NULL, NULL_POINTER_PASSED_TO_FUNC, return NULL_POINTER_PASSED_TO_FUNC);
    MYASSERT(source_text->string_array  != NULL, NULL_POINTER_PASSED_TO_FUNC, return NULL_POINTER_PASSED_TO_FUNC);

    const char *unknown_word = "qwerty";

    for (size_t i = 0; i < AMOUNT_TESTS; i++)
    {
        for (size_t j = 0; j < (source_text->number_lines); j++)
        {
            volatile TYPE_ELEMENT_LIST find_elem = hash_table_search(table, source_text->string_array[i].string_pointer, source_text->string_array[i].size_string - 1);
        }
    }

    return ASSERT_NO_ERROR;
}

errors_code hash_table_output_data(hash_table *table, FILE *file_output)
{
    MYASSERT(table                      != NULL, NULL_POINTER_PASSED_TO_FUNC, return NULL_POINTER_PASSED_TO_FUNC);
    MYASSERT(table->hash_function       != NULL, NULL_POINTER_PASSED_TO_FUNC, return NULL_POINTER_PASSED_TO_FUNC);
    MYASSERT(file_output                != NULL, NULL_POINTER_PASSED_TO_FUNC, return NULL_POINTER_PASSED_TO_FUNC);

    for (size_t i = 0; i < (table->size); i++)
    {
        size_t cases               = 0;
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

errors_code hash_table_dtor(hash_table *table)
{
    MYASSERT(table                != NULL, NULL_POINTER_PASSED_TO_FUNC, return NULL_POINTER_PASSED_TO_FUNC);
    MYASSERT(table->hash_function != NULL, NULL_POINTER_PASSED_TO_FUNC, return NULL_POINTER_PASSED_TO_FUNC);
    MYASSERT(table->items         != NULL, NULL_POINTER_PASSED_TO_FUNC, return NULL_POINTER_PASSED_TO_FUNC);

    for (size_t i = 0; i < table->size; i++)
        list_destructor(table->items[i].list_pointer);

    table->hash_function = NULL;
    table->size = 0;

    free(table->items);
    table->items = NULL;

    free(table);

    return ASSERT_NO_ERROR;
}
