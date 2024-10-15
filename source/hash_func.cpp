#include <stdio.h>
#include <stdint.h>
#include <immintrin.h>
#include "hash_table.h"

inline static size_t rotate_left(uint64_t num);
inline static size_t rotate_right(uint64_t num);

size_t zero_hash(const char* str, size_t length)
{
    MYASSERT(str    != NULL, NULL_POINTER_PASSED_TO_FUNC, return 0);
    MYASSERT(length != NULL, THE_LEN_IS_0,                return 0);

    return 0;
}

size_t first_letter_hash(const char* str, size_t length)
{
    MYASSERT(str    != NULL, NULL_POINTER_PASSED_TO_FUNC, return 0);
    MYASSERT(length != NULL, THE_LEN_IS_0,                return 0);

    return (size_t) str[0];
}

size_t length_hash(const char* str, size_t length)
{
    MYASSERT(str != NULL, NULL_POINTER_PASSED_TO_FUNC, return 0);

    return length;
}

size_t sum_hash(const char* str, size_t length)
{
    MYASSERT(str != NULL, NULL_POINTER_PASSED_TO_FUNC, return 0);

    size_t summ = 0;
    for(size_t i = 0; i < length; i++) {
        summ += (size_t) str[i];
    }

    return summ;
}

size_t sum_length_hash(const char* str, size_t length)
{
    MYASSERT(str != NULL, NULL_POINTER_PASSED_TO_FUNC, return 0);

    size_t summ = 0;
    for(size_t i = 0; i < length; i++) {
        summ += (size_t) str[i];
    }

    return summ / length;
}

const size_t BITS_IN_BYTE = 8;

inline static size_t rotate_right(uint64_t num)
{
    return ((num >> 1) | (num << (sizeof(num) * BITS_IN_BYTE - 1)));
}

inline static size_t rotate_left(uint64_t num)
{
    return ((num << 1) | (num >> (sizeof(num) * BITS_IN_BYTE - 1)));
}

size_t rotate_hash(const void* mem, size_t size, bool is_right)
{
    const char* c_mem = (const char*)mem;
    uint64_t hash = (uint64_t)c_mem[0];

    for (size_t i = 0; i < size; i++) {
        // rotate right
        hash = (is_right) ? rotate_right(hash) : rotate_left(hash);
        // xor
        hash = hash ^ (uint64_t)c_mem[i];
    }

    return hash;
}

size_t ror_hash(const char* str, size_t length)
{
    MYASSERT(str != NULL, NULL_POINTER_PASSED_TO_FUNC, return 0);

    return rotate_hash(str, length, 1);
}

size_t rol_hash(const char* str, size_t length)
{
    MYASSERT(str != NULL, NULL_POINTER_PASSED_TO_FUNC, return 0);

    return rotate_hash(str, length, 0);
}

size_t elf_hash(const char* str, size_t length)
{
    MYASSERT(str    != NULL, NULL_POINTER_PASSED_TO_FUNC, return 0);
    MYASSERT(length != NULL, THE_LEN_IS_0,                return 0);

    size_t h = 0, high = 0;

    while (*str)
    {
        h = (h << 4) + (size_t) *str++;
        if (high == (h & 0xF0000000))
            h ^= high >> 24;
        h &= ~high;
    }

    return h;
}

size_t crc32_hash(const char* str, size_t length)
{
    MYASSERT(str    != NULL, NULL_POINTER_PASSED_TO_FUNC, return 0);
    MYASSERT(length != NULL, THE_LEN_IS_0,                return 0);

    const size_t TABLE_LEN = 256;

    uint_least32_t crc_table[TABLE_LEN] = {};
    uint_least32_t crc = 0;

    for (size_t i = 0; i < TABLE_LEN; i++)
    {
        crc = (uint_least32_t) i;
        for (size_t j = 0; j < 8; j++)
            crc = crc & 1 ? (crc >> 1) ^ 0xEDB88320UL : crc >> 1;

        crc_table[i] = crc;
    };

    crc = 0xFFFFFFFFUL;

    while (length--)
        crc = crc_table[(crc ^ (uint_least32_t) *str++) & 0xFF] ^ (crc >> 8);

    return crc ^ 0xFFFFFFFFUL;
}

size_t crc32_hash_sse(const char* str, size_t length)
{
    MYASSERT(str    != NULL, NULL_POINTER_PASSED_TO_FUNC, return 0);
    MYASSERT(length != NULL, THE_LEN_IS_0,                return 0);

    uint_least32_t crc = 0;

    for (size_t i = 0; i < length; i++)
    {
        crc = _mm_crc32_u8 (crc, (unsigned char)str[i]);
    };

    return crc ^ 0xFFFFFFFFUL;
}
