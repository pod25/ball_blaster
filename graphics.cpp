/*
 * File: graphics.cpp
 */
#include "common.h"

/*
 * Image load function
 */
sdl_surface* load_image(str filename) {
	sdl_surface* loaded_image = NULL;
	
	// Optimized version of the image
	sdl_surface* optimized_image = NULL;

	// Load the image
	loaded_image = IMG_Load(filename.c_str());

	// Image loaded?
	if(loaded_image != NULL) {
		// Create an optimized image
		optimized_image = SDL_DisplayFormat(loaded_image);

		// Free the old image
		SDL_FreeSurface(loaded_image);
	}

	// Return the optimized image
	return optimized_image;
}

/*
 * Apply one surface to another at coordinate (x,y)
 */
void apply_surface(int x, int y, sdl_surface* src, sdl_surface* dest) {
	// Make a temporary rectangle to hold the offsets
	SDL_Rect offset;
	offset.x = x;
	offset.y = y;

	// Blit the surface
	SDL_BlitSurface(src, NULL, dest, &offset);
}