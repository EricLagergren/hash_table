#include <stdio.h>

#include "hash.h"

uint64_t hasher(void *val) {
	uint64_t x = (uint64_t)val;
	x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = ((x >> 16) ^ x);
    return x;
}

bool compar(void *a, void *b) {
	return (long)(a) == (long)(b);
}

#define INT(v) ((void*)(v))

int main(void) {
	Table *t = ht_new(hasher, compar, NULL, 100);

	long v;
	for (long i = 1; i < 100; ++i) {
		ht_set(t, INT(i), INT(i));
		v = (long)ht_get(t, INT(i));
		if (v != i) {
			printf("wanted: %ld, got: %ld\n", i, v);
		}
	}

	ht_delete(t, INT(5));
	v = (long)ht_get(t, INT(5));
	if (v != 0) {
		printf("wanted 0, got: %ld\n", v);
	}

	ht_free(t);
}