#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct Entry Entry;
struct Entry {
	void *key;
	void *val;
	bool empty;
	Entry *next;
};

// ht_hasher should return the hash of a key.
typedef uint64_t (*ht_hasher)(void *);

// ht_comparator should return true if both
// parameters are equal to each other.
typedef bool (*ht_comparator)(void *, void *);

// ht_freer should free the pointer.
typedef void (*ht_freer)(void*);

typedef struct {
	Entry 		  **buckets;
	size_t 		  nbuckets;
	size_t 		  nitems;
	size_t 		  ncolls;

	ht_hasher 	  hasher;
	ht_comparator compar;
	ht_freer 	  freer;
} Table;


Table* ht_new(ht_hasher h, ht_comparator c, ht_freer f, size_t size);
void   ht_init(Table *t, ht_hasher h, ht_comparator c, ht_freer f, size_t size);
void   ht_free(Table *t);
int    ht_resize(Table *t);
int    ht_set(Table *t, void *key, void *val);
bool   ht_has(Table *t, void *key);
void*  ht_get(Table *t, void *key);
void   ht_delete(Table *t, void *key);