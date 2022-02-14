#pragma once
#include <NGE/nge.h>
#include <NB/nb.h>
#include "gamecontent.h"

// Gets the direction given the vector based on game's coordinate system.
enum direction cgfGetVectorDirection(vector);
// Gets the row according to formula given in project1_sprites.md.
long cgfGetDirectionalRow(long current_row, enum direction);
// Transforms world coordinates to screen coordinates.
vector cgfWorldToScreen(vector camera, unsigned long scale, vector worldCoord);
// Transforms screen coordinates to world coordinates.
vector cgfScreenToWorld(vector camera, unsigned long scale, vector screenCoord);

// Change room state

// Removes object from room object list at index.
void cgfRemoveObject(struct nge_room*, unsigned long index);
// Removes entity from room entity list at index.
void cgfRemoveEntity(struct nge_room*, unsigned long index);
// Spawns an object at position and reallocs room's object list accordingly.
int cgfSpawnObject(struct nge_room*, enum object, vector position);
// Spawns an entity at position and reallocs room's entity list accordingly.
int cgfSpawnEntity(struct nge_room*, enum entity, vector position);
// Moves the room's camera position to follow the following_position
// and clamps to the border if the camera would have shown beyond it.
void cgfMoveCamera(struct nge_room*, vector follow_position);

// Inputs

// Enable game input.
void cgfEnableInput();
// Disable game input.
void cgfDisableInput();

// Gets the position of the mouse in room coordinates.
vector cgfGetMouseRoomPos(vector camera, unsigned long scale);
// Controls the entity and sets its rows according to the inputs given.
void cgfControlEntity(struct nge_entity*, dimension speed);
// Gets the velocity vector that the player is trying to input.
vector cgfGetInputVelocity(dimension magnitude);
// Only true if they key was not pressed before and now is being pressed.
bool cgfIsKeyTapped(enum nb_key);