#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash_table.h"

errors_code hash_table_ctor(hash_table *table, size_t size, size_t (*hash_function) (ht_key_t, ht_value_t))
{
    MYASSERT(table         != NULL, NULL_POINTER_PASSED_TO_FUNC, return NULL_POINTER_PASSED_TO_FUNC);
    MYASSERT(hash_function != NULL, NULL_POINTER_PASSED_TO_FUNC, return NULL_POINTER_PASSED_TO_FUNC);

    table->size = size;
    table->hash_function = hash_function;
    table->items = (hash_table_item *) calloc(table->size, sizeof(hash_table_item));

    for (size_t i = 0; i < table->size; i++)
    {
        hash_table_init_item((table->items)+i);
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

errors_code replace_chars_to_null_character(char *text, const char *replaceable_characters)
{
    MYASSERT(text                   != NULL, NULL_POINTER_PASSED_TO_FUNC, return NULL_POINTER_PASSED_TO_FUNC);
    MYASSERT(replaceable_characters != NULL, NULL_POINTER_PASSED_TO_FUNC, return NULL_POINTER_PASSED_TO_FUNC);

    text = strpbrk(text, replaceable_characters);

    while (text != NULL)
    {
        *text = '\0';
        text++;
        text  = strpbrk(text, replaceable_characters);
    }

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
