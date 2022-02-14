# REM (REsource Manager)

REM loads and manages assets needed by the game.
The implementation of this is dependent on the
operating system. On windows, it loads assets
packed into the .exe by the resource compiler.
Other operating systems will be similar.
For undefined operating systems, a .pak filesystem
which packs all the assets neatly into .pak files 
will be used. Asset files reside in `assets/` in the
root directory.

## Memory management functions
```c
unsigned char* remAlloc(size_t len);
void remFree(unsigned char*);
void remFreeAll();
```

Rem keeps an internal array of pointers.
The remAlloc function allocates heap memory
and appends the pointer to
the internal array of pointers.
remFree frees a pointer from the list and from
memory. remFreeAll does the same but with the
whole array of pointers.

## Loading functions

```c
readonly_bytes remLoadBytes(const wchar_t*);

int remLoadFont(const wchar_t*, ascii_fontset);
image remLoadImage(const wchar_t*);
```

The loading functions decode the raw bytes
from remLoadBytes given a [format](#Formats).

## Formats

### Images
```c
image remLoadImage(const wchar_t*);
int remLoadFont(const wchar_t*, ascii_fontset);
```

A pixel is a tuple of 4 bytes that specify
the values of the red, green, blue and alpha
channels in that order.  An image starts with
two 64 bit numbers that specifies the width and
height in pixels in that order, followed by
an array of pixels.

Bytes:

```
<> = 1 byte

[8 byte width]
[8 byte height]
<R0><G0><B0><A0>
<R1><G1><B1><A1>
<R2><G2><B2><A2>
...
```

The first pixel in the array is the bottom left,
the scanline moves to the right then to
the next row. The next row is above.
