#pragma once
#include <NB/nb.h>
#include <NGE/nge.h>

typedef const unsigned char* rem_readonly_bytes;
typedef       unsigned char* rem_bytes;

// Resource loading

// Load bytes struct from resource with name
rem_readonly_bytes remLoadResourceBytes(const char* resource_name);

// Load image from resource name
image remLoadImage(const char* resource_name);
// Load font using nb
int remLoadFont(const char* resource_name, ascii_fontset out_font);

// Memory management

// Reallocates the pointer, assumed to be allocated by remAlloc to a new size
// Returns NULL if failed
void* remRealloc(void*, size_t new_len);
// Allocate bytes that will be freed on call to remFreeAll. Returns NULL if len is 0.
void* remAlloc(size_t len);
// Frees memory allocated by remAlloc
void remFree(void*);
// Frees all allocated resources
void remFreeAll();