#include <stdio.h>

#include "hash.h"

uint64_t hasher(void *val) {
	return *(uint64_t *)(val);
}

bool compar(void *a, void *b) {
	return (a != NULL && b != NULL) &&
		  *(uint64_t *)(a) == *(uint64_t *)(b);
}

int main(void) {
	Table *t = ht_new(hasher, compar, 0);

	uint64_t v;

	uint64_t k1 = 1;
	uint64_t v1 = 1;
	ht_set(t, &k1, &v1);
	v = *(uint64_t *)ht_get(t, &k1);
	printf("%lu\n", v);

	uint64_t k2 = 2;
	uint64_t v2 = 2;
	ht_set(t, &k2, &v2);
	v = *(uint64_t *)ht_get(t, &k2);
	printf("%lu\n", v);

	ht_free(t);
}