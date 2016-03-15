#include <stdio.h>

#include "hash.h"

uint64_t hasher(void *val) {
	return (uint64_t)(val);
}

bool compar(void *a, void *b) {
	return (uint64_t)(a) == (uint64_t)(b);
}

#define INT(v) ((void*)(v))

int main(void) {
	Table *t = ht_new(hasher, compar, 0);

	uint64_t v;

	ht_set(t, INT(1), INT(1));
	v = (uint64_t)ht_get(t, INT(1));
	printf("%lu\n", v);

	ht_set(t, INT(2), INT(2));
	v = (uint64_t)ht_get(t, INT(2));
	printf("%lu\n", v);

	ht_free(t);
}