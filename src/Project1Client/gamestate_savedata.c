#include "gamecontent.h"
#include "global.h"

struct data_piece {
	unsigned char* data;
	size_t len;
};

static unsigned char zero_padding[16] = { 0 };

#define SAVE_DATA_VAR(v) { .data = (unsigned char*)&v, .len = sizeof(v) }
#define SAVE_DATA_PTR(v) { .data = (unsigned char*)v, .len = sizeof(*v) }
#define PADDING(length)  { .data = zero_padding, .len = length }

#define SAVE_MAP {\
	PADDING(4)\
}

#define LEN(x) (sizeof(x)/sizeof(x[0]))

#define throwErrF(f, ...) { fprintf(stderr, f "\r\n", __VA_ARGS__);\
							retval = 1; goto exit; }
#define throwErr(f) { fprintf(stderr, f "\r\n");\
							retval = 1; goto exit; }

int loadGameData(const char* filename) {
	int retval = 0;
	FILE* file = fopen(filename, "rb");
	if (!file) throwErrF("(loading) Could not open save file \"%s\".", filename);

	// Get number of save points for backwards compatibility.
	unsigned long count = 0;
	if (fread(&count, sizeof(count), 1, file) != 1)
		throwErr("Could not read save count.");

	struct data_piece save_map[] = SAVE_MAP;
	
	for (unsigned long i = 0; i < count; i++) {
		if (fread(save_map[i].data, save_map[i].len, 1, file) != 1)
			throwErrF("Could not read save_map[%lu].", i);
	}

exit:
	if (file) fclose(file);
	return retval;
};

int saveGameData(const char* filename) {
	int retval = 0;
	FILE* file = fopen(filename, "wb");
	if (!file) throwErrF("(saving) Could not open save file \"%s\".", filename);

	struct data_piece save_map[] = SAVE_MAP;

	// Write number of save points for backwards compatibility.
	unsigned long count = LEN(save_map);
	if (fwrite(&count, sizeof(count), 1, file) != 1)
		throwErr("Could not write save count.");

	for (unsigned long i = 0; i < count; i++) {
		if (fwrite(save_map[i].data, save_map[i].len, 1, file) != 1)
			throwErrF("Could not write save_map[%lu].", i);
	}

exit:
	if (file) fclose(file);
	return retval;
}