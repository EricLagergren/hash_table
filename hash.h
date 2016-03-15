#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct Entry Entry;
struct Entry {
	void *key;
	void *val;
	uint64_t hash;
	Entry *next;
};

typedef uint64_t (*ht_hasher)(void *);
typedef bool (*ht_comparator)(void *, void *);

typedef struct {
	Entry **entries;
	size_t nentries;
	size_t nitems;
	ht_hasher hasher;
	ht_comparator compar;
} Table;


Table* ht_new(ht_hasher h, ht_comparator c, size_t size);
void ht_init(Table *t, ht_hasher h, ht_comparator c, size_t size);
void ht_free(Table *t);
int ht_set(Table *t, void *key, void *val);
bool ht_has(Table *t, void *key);
void *ht_get(Table *t, void *key);