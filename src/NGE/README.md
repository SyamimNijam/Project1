# NGE (N Game Engine)

The NGE library provides game logic, physics and animation from spritesheets.
For rendering, audio, networking it uses the NB library.

Note: Graphics functions work only between nbInit and nbQuit.

## Structures
In a game you need three C structs:
```c
struct nge_object;
struct nge_entity;
struct nge_room;
```
A room is a game level that has a grid of textured tiles,
a camera position, an array of entities and an array of objects.

An object is a static sprite in the room which may or may not block entities from moving into it.

An entity is a moving "thing" that has a sprite, pointers to update, render and collide functions as well as a pointer to custom data.

## Coordinate space

![worldcoord](../docs/nge_worldcoord.png)

A room is made up of tiles, the dimensions of the tile textures in pixels are equal to game coordinates. The bottom left of the room is the origin, (0,0).

## Tilemap format

A tilemap in game is an array of chars. From the first char
to the last in the array the game renders tiles
from left to right first, then moves on to the next row above.

![renderorder](../docs/nge_tile_render_order.png)

## Game loop

To make a level, a room struct needs to be initialized, then updated and rendered continuously in a game loop with
```c
void ngeUpdateRoom(struct nge_room*, milliseconds delta);
void ngeRenderRoom(struct nge_room*, unsigned long scale);
```

A simple game loop looks like this:
```c
milliseconds last_update = nbGetTicks();

while (1) {
	// Exit if window is closed
	nbPollEvent();
	if (!nbWindowIsUp())
		break;

	// Update cycle
	milliseconds now = nbGetTicks();
	milliseconds time_delta = now - last_update;
	ngeUpdateRoom(game.current_room, time_delta);
	last_update = now;
	
	// Render cycle
	nbClear();
	ngeRenderRoom(game.current_room, game.settings.zoom);
	nbSwapBuffers();
}
```
