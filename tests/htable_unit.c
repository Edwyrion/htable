// ==============================================================================
//                            Hash Table Unit Tests
// ==============================================================================
//
// Description: This source file contains unit tests for the hash table library
//              functions. The tests cover creating a hash table, inserting
//              key-value pairs, and handling hash collisions for integer and
//              string types. The test assertions are based on TAP.
//
// MIT License
//
// Copyright (c) 2024 Erik A. Rapp
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
// ==============================================================================

#include "htable.h"

#include <stdio.h>
#include <string.h>

static int passed;          /* number of passing tests */
static int test_index;     /* ordinal of current test */

#define RESET() do {    \
    passed = 0;         \
    test_index = 0;     \
} while (0)

#define TEST(x) do {                    \
    char *msg = "not ok";               \
    int pass = (x);                     \
    test_index++;                       \
    if (pass) {                         \
        passed++;                       \
        msg += 4;                       \
    }                                   \
    if (!pass) {                        \
        printf("%s %d - %s:%d\t%s\n",   \
        msg, test_index, __FILE__,      \
        __LINE__, #x);                  \
    }                                   \
    else {                              \
        printf("%s %d\n", msg,          \
        test_index);                    \
    }                                   \
} while (0)

// --- Hash functions for integer and string keys --- //

unsigned long hash_int (const void *key) {
    return *(int *) key;
}

// djb2 hash function for strings.
unsigned long hash_string (const void *key) {
    unsigned long hash = 5381;
    const char *str = (const char *) key;

    while (*str) {
        hash = ((hash << 5) + hash) ^ *str++;
    }

    return hash;
}

// --- Comparison functions for integer and string keys --- //

int compare_int (const void *key1, const void *key2) {
    return *(int *) key1 - *(int *) key2;
}

int compare_string (const void *key1, const void *key2) {
    return strcmp((const char *) key1, (const char *) key2);
}

// --- Unit tests for hash table functions --- //

#define HASH_MAX 1024

void test_htable_create (void) {

    htable_t *map = htable_create(HASH_MAX, NULL, NULL, NULL);

    TEST(map != NULL); // 1
    TEST(map->table != NULL); // 2
    TEST(map->size == HASH_MAX); // 3

    htable_destroy(map);
}

void test_htable_insert_int (void) {

    htable_t *map = htable_create(1, hash_int, compare_int, NULL);

    int key = 42;
    int value = 100;

    TEST(htable_insert(map, &key, &value) == 0); // 1

    int *result = htable_get(map, &key);

    TEST(*result == value); // 2

    htable_destroy(map);
}

void test_htable_insert_string (void) {

    htable_t *map = htable_create(1, hash_string, strcmp, NULL);

    char *key = "hello";
    char *value = "world";

    TEST(htable_insert(map, &key, &value) == 0); // 1

    int *result = htable_get(map, &key);

    TEST(*result == value); // 2

    htable_destroy(map);
}

void test_htable_collision_int (void) {
    
    htable_t *map = htable_create(1, hash_int, compare_int, NULL);

    int key1 = 0;
    int value1 = 100;

    int key2 = 1;
    int value2 = 200;

    int value3 = 300;

    TEST(htable_insert(map, &key1, &value1) == 0); // 1
    TEST(htable_insert(map, &key2, &value2) == 0); // 2
    TEST(htable_insert(map, &key1, &value3) == 0); // 3

    int *result1 = htable_get(map, &key1);
    int *result2 = htable_get(map, &key2);
    int *result3 = htable_get(map, &key1);

    TEST(*result1 == value3); // 4
    TEST(*result2 == value2); // 5
    TEST(*result3 == value3); // 6

    htable_destroy(map);
}

void test_htable_collision_string (void) {
        
    htable_t *map = htable_create(2, hash_string, compare_string, NULL);

    char *key1 = "hello";
    char *value1 = "world";

    char *key2 = "world";
    char *value2 = "hello";

    char *value3 = "goodbye";

    TEST(htable_insert(map, key1, value1) == 0); // 1
    TEST(htable_insert(map, key2, value2) == 0); // 2
    TEST(htable_insert(map, key1, value3) == 0); // 3

    char *result1 = htable_get(map, key1);
    char *result2 = htable_get(map, key2);
    char *result3 = htable_get(map, key1);

    TEST(strcmp(result1, value3) == 0); // 4
    TEST(strcmp(result2, value2) == 0); // 5
    TEST(strcmp(result3, value3) == 0); // 6

    htable_destroy(map);
}

// --- Main function to run the unit tests --- //

int main (void) {

    RESET();

    test_htable_create();
    test_htable_insert_int();
    test_htable_insert_string();
    test_htable_collision_int();
    test_htable_collision_string();

    printf("Passed: %d, Failed: %d\n", passed, test_index - passed);

    return passed == test_index ? 0 : 1;
}

