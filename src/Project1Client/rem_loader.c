#include "rem.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// OS dependant

#ifdef _WIN32

#define UNICODE
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#undef WIN32_LEAN_AND_MEAN
#undef UNICODE

rem_readonly_bytes remLoadResourceBytes(const char* name) {
	rem_readonly_bytes ret = NULL;

	wchar_t gbgb[256] = { 0 };
	for (unsigned long i = 0; name[i]; i++)
		gbgb[i] = name[i];
	HRSRC resinfo = FindResource(NULL, gbgb, RT_RCDATA);
	if (resinfo != NULL) {
		HGLOBAL res = LoadResource(NULL, resinfo);
		ret = LockResource(res);
	} else {
		fprintf(stderr, "(REM) Could not find resource with name \"%ls\".\r\n", gbgb);
	}
	return ret;
}

#else
#error Unimplemented for other operating systems
#endif

// Cross-platform

int remLoadFont(const char* name, ascii_fontset out_font) {
	rem_readonly_bytes data = remLoadResourceBytes(name);
	if (data == NULL || out_font == NULL) return 1;
	pixels width  = (pixels)((uint64_t*)data)[0],
	       height = (pixels)((uint64_t*)data)[1];
	nbGenerateFontset(out_font, &data[sizeof(uint64_t) * 2], width * height * 4, width);

	return 0;
}

image remLoadImage(const char* name) {
	rem_readonly_bytes data = remLoadResourceBytes(name);
	if (data == NULL) return (image){ 0 };
	pixels width  = (pixels)( ((uint64_t*)data)[0] ),
	       height = (pixels)( ((uint64_t*)data)[1] );
	return nbLoadImageFromBytes(&data[sizeof(uint64_t) * 2], height, width);
}