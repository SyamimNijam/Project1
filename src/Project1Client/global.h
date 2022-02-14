#pragma once

extern struct game_state game;

#define CURRENT_ROOM game_room[game.current_room_id]

#define WINDOW_TITLE L"Project 1"
#define WINDOW_HEIGHT 736
#define WINDOW_WIDTH 1280
#define WINDOW_RIGHT (WINDOW_WIDTH/2)
#define WINDOW_TOP (WINDOW_HEIGHT/2)
#define WINDOW_LEFT (WINDOW_WIDTH/-2)
#define WINDOW_BOTTOM (WINDOW_HEIGHT/-2)

#define FPS 100.0f