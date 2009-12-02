/*
 * File: graphics.cpp
 */
#include "common.h"

void graphics::set_refresh_flag() {
	refresh = true;
}

void graphics::update() {
	if (!refresh) return;

	// Your code here

	refresh = false;
}

graphics::graphics() {
	refresh = false;
	//screen_buffer		.init(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE);
	//background_buffer	.load("background.png");
	//wall_buffer			.load("wall.png");
	//for (uint i = 0; i < NUM_OBJECT_CLASSES; i++) object_buffers[i].load("objectssss.png");
	//ball_buffer			.load("ball.png");
}
