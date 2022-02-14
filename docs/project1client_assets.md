# Project1Client Assets

Assets from the game's perspective are mappings from 
names(string) to byte buffers(a single uchar pointer).
Where the bytes come from are handled by [REM](project1client_rem.md).

## Asset files

For each entity template and for each object template,
the game will load their [spritesheets](project1_sprites.md) as REM images
from a naming format defined in [`naming.h`](/src/Project1Client/naming.h).
Information such as column count or animation fps are defined in
code in the struct initializers in the template array.
In the game, all entity templates, object templates and rooms
have numbers assigned to them, which is their index into
their arrays which are equal to their enum values.
(see [Content definition files](/src/Project1Client/README.md#Content-definition-files))

For each room, there are two separate asset files.

- Tile set (REM image)
- Tile map

### Tile sets

Tile sets are a set of tile textures, a tile is
always a square, so the length of the tile in pixels
on both sides are equal to the height of the tile set
image. The number of tile textures in the set is
determined by the width of the texture divided by
the length of a tile. This means tile textures
are packed horizontally and there will only be
1 tile per row.

### Tile maps

A tile map is an array of tile IDs stored in the form
of unsigned chars (bytes). The horizontal and vertical
tile count are defined in C in their content files.
