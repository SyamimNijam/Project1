# Project1Client

This sub-project is the game itself.
It is named the way it is because I haven't figured out a name for it yet
and "client" because I plan to make it an mmo.
The game project is responsible for:
- Running the game loop and window loop.
- Defining formats for loading game assets.
- Loading the assets.
- Input mapping to game controls.
- Defining the GUI system.
- Resource management.

Graphics, networking and audio functions are provided by
the [NB library](../NB/README.md) while the game structures and sprites are
the responsibility of the [N Game Library](../NGE/README.md).

## Game content

All game content other than tilemaps (uchar arrays) and images are
defined in code. Images and tilemaps are stored as asset files
which are then embedded into the game binary or packed into .pak files.
Game content source files are stored in `content/`. These files define
entity functions and metadata for rooms, objects and entities.

### Content definition files

There is a file named `gamecontent.h`, all it does
is include three other header files:

```c
#include "entity.h"
#include "object.h"
#include "room.h"
```

These files and their `.c` equivalents define
game content as C code. They define it in the form
of global array variables.

```c
enum entity;
enum object;
enum room;

struct nge_entity game_entity[];
struct nge_object game_object[];
struct nge_room game_room[];
```

Each room in `game_room` is a 2D scene.
Each entity and object in their array serve as templates
to copy into the room's entity and object lists.

For example, spawning an enemy slime would look like this:

```c
game_room[ROOM_CAVE1].entity_list[0] = game_entity[ENTITY_SLIME];
game_room[ROOM_CAVE1].entity_list[0].body.x = 48;
game_room[ROOM_CAVE1].entity_list[0].body.x = 64;
```

Entities may have custom data with variable lengths,
their own update function, collision functions
and their own render function.

### How to add content to the game

Assuming you have read how [game assets](/docs/project1client_assets.md) work,
you can then begin working on static game objects and tiles without
learning how the other two libraries work. For entities however,
you may  and use pre-existing update, render and collision functions
or read the documentation for the other two libraries to make
custom behaviour functions.

## Procedures

### Adding a level

1. Enter a name for it in the room enum in `room.h`.
2. Create the necessary asset files in `assets/` and
   name them according to the level's enum value.

### Adding an object template

1. Enter a name for it in the object enum in `object.h`.
2. In `object.c`, initialize the struct with your enum's value
   as the index in the array.
3. Create the necessary asset files in `assets/` and
   name them according to the object's enum value.

### Adding an entity template

1. Enter a name for it in the entity enum in `entity.h`.
2. In `entity.c`, initialize the struct with your enum's value
   as the index in the array.
3. Create the necessary asset files in `assets/` and
   name them according to the entity's enum value.
4. If needed, make a `.c` file in `content/` for your entity's
   functions or custom structs.
5. If the entity has custom data, leave the data pointer as NULL
   and set `data_len` to the size of the custom data. The game's
   loader will dynamically allocate a unique pointer for 
   each instance of the entity and initialize the memory to 0.

Note: All source files can be found in `Project1/src/Project1Client/` unless explicitly stated otherwise.
