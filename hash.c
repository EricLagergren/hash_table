#include <stdio.h>
#include <errno.h>

#include "hash.h"

#ifndef LOAD_FACTOR
# define LOAD_FACTOR 0.75
#endif

#define GET_ENTRY(t, i) ((t)->entries[(i)])

// ht_new returns a pointer to a freshly-allocated hash table.
// If size is 0 a small starting size will be used.
// It is the caller's responsibility to free the returned pointer.
Table* ht_new(ht_hasher h, ht_comparator c, size_t size) {
	Table *t = calloc(1, sizeof(Table));
	if (t == NULL) {
		return NULL;
	}

	if (size == 0) {
		size = 16;
	}

	ht_init(t, h, c, size);

	t->entries = calloc(size, sizeof(Entry *));
	if (t->entries == NULL) {
		free(t);
		return NULL;
	}
	return t;
}

// ht_init initializes a hash table from the given storage.
// It does not allocate any memory.
void ht_init(Table *t, ht_hasher h, ht_comparator c, size_t size) {
	if (t == NULL) {
		return;
	}
	t->nentries = size;
	t->nitems = 0;
	t->hasher = h;
	t->compar = c;
}

void ht_free(Table *t) {
	Entry *ep = NULL;
	Entry *head = NULL;
	for (size_t i = 0; i < t->nentries; ++i) {
		head = GET_ENTRY(t, i);
		while ((ep = head) != NULL) {
			head = head->next;
			free(ep);
		}
	}
	free(t->entries);
	free(t);
}

// ht_set returns 0 if val was successfully set.
int ht_set(Table *t, void *key, void *val) {
	uint64_t hash = t->hasher(key);	
	size_t pos = hash % t->nentries;

	Entry **ep = &GET_ENTRY(t, pos);
	Entry *cur = NULL;
	while ((cur = *ep) != NULL) {
		// If we can find it, replace the current entry.
		if (cur->key != NULL && t->compar(cur->key, key)) {
			cur->val = val;
			return 0;
		}
		ep = &cur->next;
	}

	// Otherwise alloc a new Entry and set that.
	*ep = calloc(1, sizeof(Entry));
	if (*ep == NULL) {
		ht_free(t);
		return -ENOMEM;
	}
	(*ep)->val = val;
	(*ep)->key = key;
	++t->nitems;
	return 0;
}

// ht_has returns true if the key is found inside t.
bool ht_has(Table *t, void *key) {
	uint64_t hash = t->hasher(key);	
	size_t pos = hash % t->nentries;

	Entry *ep = GET_ENTRY(t, pos);
	while (ep != NULL) {
		if (ep->key != NULL && t->compar(ep->key, key)) {
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
	size_t pos = hash % t->nentries;

	Entry *ep = GET_ENTRY(t, pos);
	while (ep != NULL) {
		if (ep->key != NULL && t->compar(ep->key, key)) {
			return ep->val;
		}
		ep = ep->next;
	}
	return NULL;
}