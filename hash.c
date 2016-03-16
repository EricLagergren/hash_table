#include <errno.h>

#include "hash.h"

// LOAD_FACTOR is the number of items to number of buckets ratio
// that triggers a table resize.
#ifndef LOAD_FACTOR
# define LOAD_FACTOR 0.75
#endif

// GROW_FACTOR is how many times the current size the table grows
// to on a resize.
#ifndef GROW_FACTOR
# define GROW_FACTOR 2.0
#endif

#ifdef __GNUC__
# define UNUSED(x) UNUSED_ ## x __attribute__((__unused__))
#else
# define UNUSED(x) UNUSED_ ## x
#endif

// null_freer is a noop free.
static void null_freer(void* UNUSED(a)) {}

// create_entry creates a newly-allocated entry.
static Entry* create_entry(void *key, void *val) {
	Entry *ent = calloc(1, sizeof(Entry));
	if (ent == NULL) {
		return NULL;
	}
	ent->val = val;
	ent->key = key;
	return ent;
}

// free_entry frees e with f.
static void free_entry(Entry *e, ht_freer f) {
	f(e->key);
	f(e->val);
	free(e);
}

// ht_new returns a pointer to a freshly-allocated hash table.
// If size is 0 a small starting size will be used.
// It is the caller's responsibility to free the returned pointer.
Table* ht_new(ht_hasher h, ht_comparator c, ht_freer f, size_t size) {
	Table *t = calloc(1, sizeof(Table));
	if (t == NULL) {
		return NULL;
	}

	if (size == 0) {
		size = 16;
	}

	ht_init(t, h, c, f, size);

	t->buckets = calloc(size, sizeof(Entry *));
	if (t->buckets == NULL) {
		free(t);
		return NULL;
	}
	return t;
}

// ht_init initializes a hash table from the given storage.
// It does not allocate any memory.
void ht_init(Table *t, ht_hasher h, ht_comparator c, ht_freer f, size_t size) {
	if (t == NULL) {
		return;
	}
	t->nbuckets = size;
	t->nitems   = 0;
	t->hasher   = h;
	t->compar   = c;

	if (f == NULL) {
		f = null_freer;
	}
	t->freer = f;
}

// ht_free frees t.
void ht_free(Table *t) {
	Entry *ep = NULL;
	Entry *head = NULL;
	for (size_t i = 0; i < t->nbuckets; ++i) {
		head = t->buckets[i];
		while ((ep = head) != NULL) {
			head = head->next;
			free_entry(ep, t->freer);
		}
	}
	free(t->buckets);
	free(t);
}

// ht_set returns 0 if val was successfully set.
int ht_set(Table *t, void *key, void *val) {
	if (ht_resize(t) < 0) {
		return -ENOMEM;
	}

	uint64_t hash = t->hasher(key);
	size_t pos = hash % t->nbuckets;

	Entry **ep = &(t->buckets[pos]);
	Entry *cur = NULL;
	while ((cur = *ep) != NULL) {
		// If we can find it, replace the current entry...
		if (cur->key != NULL && t->compar(cur->key, key)) {
			cur->val = val;
			return 0;
		}
		ep = &cur->next;
	}

	// ...otherwise allocate a new Entry and set that.
	*ep = create_entry(key, val);
	if (ep == NULL) {
		return -ENOMEM;
	}
	++t->nitems;
	return 0;
}

// ht_has returns true if the key is found inside t.
bool ht_has(Table *t, void *key) {
	uint64_t hash = t->hasher(key);
	size_t pos = hash % t->nbuckets;

	Entry *ep = t->buckets[pos];
	while (ep != NULL) {
		if (t->compar(ep->key, key)) {
			return true;
		}
		ep = ep->next;
	}
	return false;
}

// ht_get returns a pointer to the data corresponding with the given
// key. The return value is not guaranteed to be not-NULL.
void *ht_get(Table *t, void *key) {
	uint64_t hash = t->hasher(key);
	size_t pos = hash % t->nbuckets;

	Entry *ep = t->buckets[pos];
	while (ep != NULL) {
		if (t->compar(ep->key, key)) {
			return ep->val;
		}
		ep = ep->next;
	}
	return NULL;
}

// ht_resize resizes the table to GROW_FACTOR * the current size.
int ht_resize(Table *t) {
	if (((double)(t->nitems) / (double)(t->nbuckets)) < LOAD_FACTOR) {
		return 0;
	}

	size_t old_size = t->nbuckets;
	size_t new_size = old_size * GROW_FACTOR;
	t->nbuckets = new_size;

	Entry **nb = calloc(new_size, sizeof(Entry *));
	if (nb == NULL) {
		ht_free(t);
		return -ENOMEM;
	}

	Entry **tmp = t->buckets;
	t->buckets = nb;
	nb = tmp;

	Entry *ep = NULL;
	Entry *head = NULL;
	for (size_t i = 0; i < old_size; ++i) {
		head = nb[i];
		while ((ep = head) != NULL) {
			head = head->next;
			if (ht_set(t, ep->key, ep->val) < 0) {
				free(nb);
				return -ENOMEM;
			}
			free(ep);
		}
	}
	free(nb);
	return 0;
}