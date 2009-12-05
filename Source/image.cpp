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

void base_image::apply(base_image &dest, int x, int y, SDL_Rect *src_part) {
	if (!     _sdl_srf) throw invalid_argument("Image being applied not loaded");
	if (!dest._sdl_srf) throw invalid_argument("Image being applied on not loaded");
	// Make a temporary rectangle to hold the offsets
	SDL_Rect offset;
	offset.x = x;
	offset.y = y;
	// Blit the surface
	if (SDL_BlitSurface(_sdl_srf, src_part, dest._sdl_srf, &offset) < 0)
		throw exception("Couldn't blit image");
}

void base_image::apply(int x, int y, SDL_Rect *src_part) {
	apply(gra.get_screen_buffer(), x, y, src_part);
}

/*
 * image class methods
 */

void image::  lock() {if (SDL_MUSTLOCK(_sdl_srf) && SDL_LockSurface(_sdl_srf) == -1) throw exception("Couldn't lock image");}
void image::unlock() {if (SDL_MUSTLOCK(_sdl_srf)) SDL_UnlockSurface(_sdl_srf);}

void image::load(string filename) {
	filename = "Images/" + filename;
	// Free old image if any
	if (_sdl_srf) SDL_FreeSurface(_sdl_srf);
	// Load the image
	SDL_Surface* loaded_image = IMG_Load(filename.c_str());
	if(loaded_image != NULL) {
		// Create an optimized image
		_sdl_srf = SDL_DisplayFormatAlpha(loaded_image);
		// Free the old image
		SDL_FreeSurface(loaded_image);
	}
	if (!_sdl_srf) throw exception(("Couldn't load image: " + filename).c_str());
}

void image::generate_rect(int w, int h) {//, SDL_Color color) {
	// Free old image if any
	if (_sdl_srf) SDL_FreeSurface(_sdl_srf);
	_sdl_srf = SDL_CreateRGBSurface(gra.IMAGE_FLAGS, w, h, gra.SCREEN_BPP, gra.RMASK, gra.GMASK, gra.BMASK, gra.AMASK);
	//if (color.r != 0 || color.g != 0 || color.b != 0) set_color(color);
	if (!_sdl_srf) throw exception("Couldn't generate rectangular image");
}

void image::generate_text(string text, font &text_font, SDL_Color text_color) {
	// Free old image if any
	if (_sdl_srf) SDL_FreeSurface(_sdl_srf);
	_sdl_srf = TTF_RenderText_Blended(text_font.get_sdl_font(), text.c_str(), text_color);
	if (!_sdl_srf) throw exception("Couldn't generate image from text with given settings");
}

void image::free() {
	if (_sdl_srf) SDL_FreeSurface(_sdl_srf);
	else          throw exception("Trying to free image that is not loaded");
	_sdl_srf = NULL;
}

void image::set_alpha(Uint8 a, bool enabled) {
	if (SDL_SetAlpha(_sdl_srf, (enabled ? SDL_SRCALPHA : 0) | SDL_RLEACCEL, a+(a==127)) == -1)
		throw exception("Couldn't set alpha");
}

void image:: enable_alpha() {
	set_alpha(_sdl_srf->format->alpha, true );
}
void image::disable_alpha() {
	set_alpha(_sdl_srf->format->alpha, false);
}

void image::fill_rect(SDL_Rect *dstrect, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
	Uint32 pixel = SDL_MapRGBA(_sdl_srf->format, r, g, b, a);
	if(SDL_FillRect(_sdl_srf, dstrect, pixel)) throw exception("Couldn't fill rectangle in image");
	/*
	Uint8  Bpp   = _sdl_srf->format->BytesPerPixel;
	Uint16 pitch = _sdl_srf->pitch;
	lock();
	int x, y;
	for (y = 0; y < _sdl_srf->h; y++)
		for (x = 0; x < _sdl_srf->w; x++)
			*(Uint32*)((byte*)_sdl_srf->pixels + x*Bpp + y*pitch) = pixel;
	unlock();
	*/
}
void image::set_color(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
	fill_rect(NULL, r, g, b, a);
	/*
	Uint32 pixel = SDL_MapRGBA(_sdl_srf->format, r, g, b, a);
	Uint8  Bpp   = _sdl_srf->format->BytesPerPixel;
	Uint16 pitch = _sdl_srf->pitch;
	lock();
	int x, y;
	for (y = 0; y < _sdl_srf->h; y++)
		for (x = 0; x < _sdl_srf->w; x++)
			*(Uint32*)((byte*)_sdl_srf->pixels + x*Bpp + y*pitch) = pixel;
	unlock();
	*/
}

//void image::set_color(SDL_Color color, Uint8 alpha) {set_color(color.r, color.g, color.b, alpha);}

/*
 *	Clear buffer (to total transparency)
 */
void image::clear() {set_color(0, 0, 0, 0);}

// This function is not used, but illustrates how a SDL_Color can be transformed to an Uint32
/*void image::set_pixel(int x, int y, SDL_Color color, Uint8 alpha) {
	if (x < 0 || y < 0 || x >= _sdl_srf->w || y >= _sdl_srf->h) throw exception("Trying to set pixel outside of image");
	Uint32 pixel = ((Uint32)color.r >> gra.RLOSS << gra.RSHIFT) + ((Uint32)color.g >> gra.GLOSS << gra.GSHIFT) +
		((Uint32)color.b >> gra.BLOSS << gra.BSHIFT) + ((Uint32)alpha >> gra.ALOSS << gra.ASHIFT) ;
	((byte*)(_sdl_srf->pixels))[x*_sdl_srf->format->BytesPerPixel + y*_sdl_srf->pitch] = pixel;
}*/

image::image(string filename) { // Constructor using a file name
	load(filename);
}

image::image(int w, int h) {//, SDL_Color color) {
	generate_rect(w, h);//, color);
}

image::image(string text, font &text_font, SDL_Color text_color) { // Constructor using a file name
	generate_text(text, text_font, text_color);
}

image::~image() { // Destructor
	if (_sdl_srf) SDL_FreeSurface(_sdl_srf);
}

/*
 * video_mode class methods
 */
void video_mode::init(int width, int height, int bpp, Uint32 flags) {
	if (_sdl_srf) throw exception("Video mode already initiated");
	_sdl_srf = SDL_SetVideoMode(width, height, bpp, flags);
	if (!_sdl_srf) throw exception("Couldn't initiate video mode");
}

void video_mode::flip() {
	if(SDL_Flip(_sdl_srf)) throw exception("Couldn't flip screen buffer");
}

video_mode::video_mode(int width, int height, int bpp, Uint32 flags) {
	init(width, height, bpp, flags);	
}
