/*
 * File: image.cpp
 */
#include "common.h"

/*
 * base_image class methods
 */
bool base_image::empty() {
	return !_sdl_srf;
}

void base_image::apply(base_image dest, int x, int y, SDL_Rect *src_part) {
	if (!     _sdl_srf) throw invalid_argument("Image applying not loaded");
	if (!dest._sdl_srf) throw invalid_argument("Image applyed on not loaded");
	// Make a temporary rectangle to hold the offsets
	SDL_Rect offset;
	offset.x = x;
	offset.y = y;
	// Blit the surface
	SDL_BlitSurface(_sdl_srf, src_part, dest._sdl_srf, &offset);
}

/*
 * image class methods
 */
bool image::load(string filename) {
	// Free old image if any
	if (_sdl_srf) SDL_FreeSurface(_sdl_srf);
	// Load the image
	SDL_Surface* loaded_image = IMG_Load(filename.c_str());
	if(loaded_image != NULL) {
		// Create an optimized image
		_sdl_srf = SDL_DisplayFormat(loaded_image);
		// Free the old image
		SDL_FreeSurface(loaded_image);
	}
	// Return true if succeeded
	return !!_sdl_srf;
}

void image::free() {
	if (_sdl_srf) SDL_FreeSurface(_sdl_srf);
	else          throw exception("No image is loaded");
	_sdl_srf = NULL;
}

image::image(string filename) { // Constructor using a file name
	load(filename);
}

image::~image() { // Destructor
	if (_sdl_srf) SDL_FreeSurface(_sdl_srf);
}

/*
 * video_mode class methods
 */
bool video_mode::init(int width, int height, int bpp, Uint32 flags) {
	if (_sdl_srf) throw exception("Video mode already initiated");
	_sdl_srf = SDL_SetVideoMode(width, height, bpp, flags);
	return !!_sdl_srf;
}

int video_mode::flip() {
	return SDL_Flip(_sdl_srf);
}

video_mode::video_mode(int width, int height, int bpp, Uint32 flags) {
	init(width, height, bpp, flags);	
}
