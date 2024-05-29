# Generic hash table implementation in C
This library provides basic C implementation of a generic hash table using a linked list for collision resolution. To use this library, user must provide a hash function and a comparison function for the keys. It is recommended to implement an additional function wrapper for the retrieval of values from the hash map for improved type safety instead of type casting every single time.

This library also includes some basic unit tests for creation, destruction, and insertions and retrievals for integers and string types.

## Compiling
```
$ make
```

## Function calls
```C
/// @brief Create a hash table with the specified size.
/// @param size Total number of nodes in the hash table.
/// @param hash User-defined hash function for the keys.
/// @param keq User-defined comparison function for the keys.
/// @return Pointer to the allocated hash table, NULL on failure.
htable_t *htable_create (size_t size, htable_hash_t hash, htable_keq_t keq, const struct callbacks *cbs);

/// @brief Destroy the hash table and free resources.
/// @param table The hash table to destroy.
void htable_destroy (htable_t *table);

/// @brief Insert a key-value pair into the hash table.
/// @param table The hash table to insert the key-value pair into.
/// @param key The key for the hash node.
/// @param value The value for the hash node.
/// @return 0 on success, -1 on invalid input, -2 on memory allocation failure.
int htable_insert (htable_t *table, const void *key, const void *value);

/// @brief Remove a key-value pair from the hash table.
/// @param table The hash table to remove the key-value pair from.
/// @param key The key for the hash node.
/// @return 0 on success, -1 on failure.
int htable_remove (htable_t *table, const void *key);

/// @brief Retrieve a value from the hash table.
/// @param table The hash table to retrieve the value from.
/// @param key The key for the hash node.
/// @return Pointer to the value on success, NULL on failure.
void *htable_get (htable_t *table, const void *key);
```

## Example
```C
void print_value(htable_t *map, int key)
{
    int* value = htable_get(map, &key);

    if (value != NULL)
    {
        (void) printf("(%d/%u, %d)\n", key, get_hash(map, &key), *value);
        return;
    }

    (void) printf("(%d/%u, NAN)\n", key, get_hash(map, &key));
}

int main(void)
{
    htable_t *map = htable_create(2, hash, compare);

    int* arr = malloc(4 * sizeof(*arr));
    int* keys = malloc(4 * sizeof(*keys));

    arr[0] = 0; keys[0] = 0;
    arr[1] = 1; keys[1] = 1;
    arr[2] = 2; keys[2] = 2;
    arr[3] = 3; keys[3] = 3;

    (void) htable_insert(map, &keys[0], &arr[0]);
    (void) htable_insert(map, &keys[1], &arr[1]);
    (void) htable_insert(map, &keys[2], &arr[2]);
    (void) htable_insert(map, &keys[3], &arr[3]);

    for (int i = 0; i < 4; i++) {
        print_value(map, i);
    }
    (void) puts("");

    (void) htable_insert(map, &keys[1], &arr[3]);

    for (int i = 0; i < 4; i++) {
        print_value(map, i);
    }
    (void) puts("");
    
    (void) htable_remove(map, &keys[1]);
    (void) htable_remove(map, &keys[2]);

    for (int i = 0; i < 4; i++) {
        print_value(map, i);
    }
    (void) puts("");

    return 0;
}
```
This example should return:
```
$ (0/0, 0)
$ (1/1, 1)
$ (2/0, 2)
$ (3/1, 3)
$ 
$ (0/0, 0)
$ (1/1, 3)
$ (2/0, 2)
$ (3/1, 3)
$ 
$ (0/0, 0)
$ (1/1, NAN)
$ (2/0, NAN)
$ (3/1, 3)
```
