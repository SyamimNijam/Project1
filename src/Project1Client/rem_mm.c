#include "rem.h"
#include <stdlib.h>

static unsigned long rem_ref_count = 0;
static void** rem_ref = NULL;

void* remRealloc(void* old_pointer, size_t new_len) {
	void* new_pointer = realloc(old_pointer, new_len);
	if (new_pointer != NULL) {
		for (unsigned long i = 0; i < rem_ref_count; i++) {
			if (rem_ref[i] == old_pointer)
				rem_ref[i] = new_pointer;
		}
	} else
		fprintf(
			stderr,
			"remRealloc failed when passed %I64d bytes.\r\n",
			new_len
		);
	return new_pointer;
}

void* remAlloc(size_t len) {
	if (len == 0) return NULL;
	void** test = realloc(rem_ref, sizeof(*rem_ref) * ++rem_ref_count);
	if (!test) {
		fprintf(stderr, "remAlloc failed when passed %I64d bytes.\r\n", len);
		rem_ref_count--;
		return NULL;
	} else {
		rem_ref = test;
		rem_ref[rem_ref_count - 1] = malloc(len);
		return rem_ref[rem_ref_count - 1];
	}
}

void remFree(void* p) {
	if (p) {
		void* pt = p; // Disables weird "uninitialized memory" warning
		free(pt);
		for (unsigned long i = 0; i < rem_ref_count; i++) {
			if (p && p == rem_ref[i])
				rem_ref[i] = NULL;
		}
	}
}

void remFreeAll() {
	for (unsigned long i = 0; i < rem_ref_count; i++) free(rem_ref[i]);
	free(rem_ref);
	rem_ref_count = 0;
}