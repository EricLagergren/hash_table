# hash_table
A relatively simple hash table written in C.

## Docs

```
// LOAD_FACTOR is the number of items to number of buckets ratio
// that triggers a table resize.
#define LOAD_FACTOR 0.75

// GROW_FACTOR is how many times the current size the table grows
// to on a resize.
#define GROW_FACTOR 2.0

// ht_hasher should return the hash of a key.
typedef uint64_t (*ht_hasher)(void *);

// ht_comparator should return true if both
// parameters are equal to each other.
typedef bool (*ht_comparator)(void *, void *);

// ht_freer should free the pointer.
typedef void (*ht_freer)(void*);

// ht_new returns a pointer to a freshly-allocated hash table.
// If size is 0 a small starting size will be used.
// It is the caller's responsibility to free the returned pointer.
Table* ht_new(ht_hasher h, ht_comparator c, ht_freer f, size_t size);

// ht_init initializes a hash table from the given storage.
// It does not allocate any memory.
void ht_init(Table *t, ht_hasher h, ht_comparator c, ht_freer f, size_t size);

// ht_free frees t.
void ht_free(Table *t);

// ht_set returns 0 if val was successfully set.
int ht_set(Table *t, void *key, void *val);

// ht_has returns true if the key is found inside t.
bool ht_has(Table *t, void *key);

// ht_get returns a pointer to the data corresponding with the given
// key. The return value is not guaranteed to be not-NULL.
void *ht_get(Table *t, void *key);

// ht_resize resizes the table to GROW_FACTOR * the current size.
int ht_resize(Table *t);

// ht_delete removes an entry from the table.
void ht_delete(Table *t, void* key);
```

## License
CC0 1.0 Universal