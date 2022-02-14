# Sprites

Sprites are structs defined in `NGE/sprite.h`.

They have a column ratio and a row ratio, these are used
to determine which portion of the spritesheet to render as
a single frame. In NGE, there is a function called
`updateSprite` which takes in a pointer to a struct
and the number of milliseconds between calls to the
function. This function is required to be called
often enough for the sprite's fps to be rendered properly.
The function advances the frame by incrementing the column ratio numerator
up until (_col\_max_-1) then resets the column
to 0 without letting it be _col\_max_. Note that the row
of the sprite isn't affected and can be used to
select the playing animation.

## Spritesheets

A spritesheet is a single image that contains frames
used for animation. Frames are organized into rows
and columns. The width and height of all frames are the
same. Where the columns and rows start
(`row.num = 0` and `col.num = 0`) depend on the 
coordinate system of the graphics library being used (NB).

## Project1Client

Rows are changed programatically and determine the
animation that is playing. For entities with different
rows for different directions, there is a convention:

`row = selected_animation * 4 + direction_enum`

The direction enum is written in `Project1Client/game.h`:

```c
enum direction {
	DIRECTION_S, // 0
	DIRECTION_E, // 1
	DIRECTION_N, // 2
	DIRECTION_W, // 3

	DIRECTION_DEFAULT = DIRECTION_S
};
```

For animations that don't have direction,
simply add padding rows to a multiple of 4.
