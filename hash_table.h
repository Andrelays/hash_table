#ifndef HASH_TABLE_H_INCLUDED
#define HASH_TABLE_H_INCLUDED

#include <stdint.h>
#include "list.h"
#include "libraries/Stack/myassert.h"

const ssize_t HASH_table_MAX_SIZE = 1500000;

typedef const char *ht_key_t;
typedef size_t      ht_value_t;

struct hash_table_item {
    struct list*        list_pointer;
    size_t              amount;
};

struct hash_table {
    hash_table_item*    items;
    size_t              size;
    size_t              (*hash_function) (ht_key_t, ht_value_t);
};

errors_code       replace_chars_to_null_character(char *text, const char *replaceable_characters);
errors_code       hash_table_ctor(hash_table *table, size_t size, size_t (*hash_function) (ht_key_t, ht_value_t));
errors_code       hash_table_init_item(hash_table_item* item);
hash_table       *new_pointer_hash_table();
errors_code       hash_table_add(hash_table *table, ht_key_t, ht_value_t);
errors_code       hash_table_dtor(hash_table *table);
TYPE_ELEMENT_LIST hash_table_search(hash_table *table, ht_key_t key, ht_value_t value);

size_t rotate_hash(const void* mem, size_t size, bool is_right);

size_t zero_hash         (const char* str, size_t length);
size_t first_letter_hash (const char* str, size_t length);
size_t length_hash       (const char* str, size_t length);
size_t sum_hash          (const char* str, size_t length);
size_t sum_length_hash   (const char* str, size_t length);
size_t ror_hash          (const char* str, size_t length);
size_t rol_hash          (const char* str, size_t length);
size_t elf_hash          (const char* str, size_t length);
size_t crc32_hash        (const char* str, size_t length);


#endif //HASH_TABLE_H_INCLUDED
