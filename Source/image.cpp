/*
 * File: image.cpp
 */
#include "common.h"

/*
 * base_image class methods
 */

void base_image::  lock() {if (SDL_MUSTLOCK(_sdl_srf) && SDL_LockSurface(_sdl_srf) == -1) throw exception("Couldn't lock image");}
void base_image::unlock() {if (SDL_MUSTLOCK(_sdl_srf)) SDL_UnlockSurface(_sdl_srf);}
bool base_image::empty() {return !_sdl_srf;}
SDL_Surface* base_image::get_sdl_srf() {return _sdl_srf;}

/*
 * image class methods
 */

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
		if (!_sdl_srf) sdl_obj.error("Couldn't optimize loaded image");
	}
	else sdl_obj.error("Couldn't load image: " + filename);
}

void image::generate_rect(int w, int h) {//, SDL_Color color) {
	// Free old image if any
	if (_sdl_srf) SDL_FreeSurface(_sdl_srf);
	_sdl_srf = SDL_CreateRGBSurface(gra.IMAGE_FLAGS, w, h, gra.SCREEN_BPP, gra.RMASK, gra.GMASK, gra.BMASK, gra.AMASK);
	//if (color.r != 0 || color.g != 0 || color.b != 0) set_color(color);
	if (!_sdl_srf) sdl_obj.error("Couldn't generate rectangular image");
}

void image::generate_text(string text, font &text_font, SDL_Color text_color) {
	// Free old image if any
	if (_sdl_srf) SDL_FreeSurface(_sdl_srf);
	_sdl_srf = TTF_RenderText_Blended(text_font.get_sdl_font(), text.c_str(), text_color);
	if (!_sdl_srf) sdl_obj.error("Couldn't generate image from text");
}

void image::free() {
	if (_sdl_srf) SDL_FreeSurface(_sdl_srf);
	else          throw exception("Trying to free image that is not loaded");
	_sdl_srf = NULL;
}

void image::apply(base_image &dest, Sint16 x, Sint16 y, SDL_Rect *src_part) {
	SDL_Surface* dest_srf = dest.get_sdl_srf();
	if (!_sdl_srf) throw invalid_argument("Image being applied not loaded");
	if (!dest_srf) throw invalid_argument("Image being applied on not loaded");
	// Make a temporary rectangle to hold the offsets
	SDL_Rect offset;
	offset.x = x;
	offset.y = y;
	if (!(_sdl_srf->flags & SDL_SRCALPHA && _sdl_srf->format->Amask &&
		  dest_srf->flags & SDL_SRCALPHA && dest_srf->format->Amask ) &&
		  alpha == 255) {	  
		// Blit the surface using SDL's build in blit function
		if (SDL_BlitSurface(_sdl_srf, src_part, dest_srf, &offset) < 0)
			sdl_obj.error("Couldn't blit image");
	}
	else { // Blit the surface by accessing the pixel data
		// Control color depth
		if (_sdl_srf->format->BitsPerPixel != 32) throw invalid_argument("Source image has invalid pixel format");
		if (dest_srf->format->BitsPerPixel != 32) throw invalid_argument("Destination image has invalid pixel format");
		// Constrol the masks of the surfaces
		Uint32 samask = _sdl_srf->format->Amask;
		Uint32 damask = dest_srf->format->Amask;
		if (_sdl_srf->format->Rmask != gra.RMASK ||
			_sdl_srf->format->Gmask != gra.GMASK ||
			_sdl_srf->format->Bmask != gra.BMASK ||
			_sdl_srf->format->Amask != gra.AMASK && samask) throw invalid_argument("Source image has invalid channel masks");
		if (dest_srf->format->Rmask != gra.RMASK ||
			dest_srf->format->Gmask != gra.GMASK ||
			dest_srf->format->Bmask != gra.BMASK ||
			dest_srf->format->Amask != gra.AMASK && damask) throw invalid_argument("Destination image has invalid channel masks");
		// Get pitches
		Uint16  src_pitch	= _sdl_srf->pitch;
		Uint16 dest_pitch	= dest_srf->pitch;
		// Get surface to write to in destination
		Sint16 sx1, dx1 =       dest_srf->clip_rect.x;
		Sint16 sy1, dy1 =       dest_srf->clip_rect.y;
		Sint16 sx2, dx2 = dx1 + dest_srf->clip_rect.w;
		Sint16 sy2, dy2 = dy1 + dest_srf->clip_rect.h;
		if (dx1 < 0) dx1 = 0;
		if (dy1 < 0) dy1 = 0;
		if (dx2 > dest_srf->w) dx2 = dest_srf->w;
		if (dy2 > dest_srf->h) dy2 = dest_srf->h;
		// Get surface to write from in source
		if (src_part) {
			sx1 =       src_part->x;
			sy1 =       src_part->y;
			sx2 = sx1 + src_part->w;
			sy2 = sy1 + src_part->h;
			if (sx1 < 0) sx1 = 0;
			if (sy1 < 0) sy1 = 0;
			if (sx2 > _sdl_srf->w) sx2 = _sdl_srf->w;
			if (sy2 > _sdl_srf->h) sy2 = _sdl_srf->h;
		}
		else {
			sx1 = 0;
			sy1 = 0;
			sx2 = _sdl_srf->w;
			sy2 = _sdl_srf->h;
		}
		// Calculate overlapping surfaces
		if (sx1 + x < dx1) sx1 = dx1 - x; //Increase source      x1 to equality;
		else               dx1 = sx1 + x; //Increase destination x1 to equality;
		if (sy1 + y < dy1) sy1 = dy1 - y; //Increase source      y1 to equality;
		else               dy1 = sy1 + y; //Increase destination y1 to equality;
		if (sx2 + x > dx2) sx2 = dx2 - x; //Decrease source      x2 to equality;
		else               dx2 = sx2 + x; //Decrease destination x2 to equality;
		if (sy2 + y > dy2) sy2 = dy2 - y; //Decrease source      y2 to equality;
		else               dy2 = sy2 + y; //Decrease destination y2 to equality;
		// Calculate start addresses and definitive drawing surface dimensions
		byte*  src_start = (byte*)_sdl_srf->pixels + sx1*4 + sy1* src_pitch;
		byte* dest_start = (byte*)dest_srf->pixels + dx1*4 + dy1*dest_pitch;
		Sint32 w = Sint32(sx2) - Sint32(sx1);
		Sint32 h = Sint32(sy2) - Sint32(sy1);
		if (w <= 0 || h <= 0) return;
		// Lock both surfaces for pixel access if necessary
		lock();
		dest.lock();
		// Blit
		Uint32 p; // Pixel value
		Uint32 sr, sg, sb, sa, dr, dg, db, da, nr, ng, nb, na; // RGBA values for source, destination and new values
		Uint8  src_pbalpha = alpha;
		for (y = 0; y < h; y++) {
			for (x = 0; x < w; x++) {
				// Extract source colors
				p = *(Uint32*)( src_start + x*4 + y* src_pitch);
				sr = (p & gra.RMASK) >> gra.RSHIFT << gra.RLOSS;
				sg = (p & gra.GMASK) >> gra.GSHIFT << gra.GLOSS;
				sb = (p & gra.BMASK) >> gra.BSHIFT << gra.BLOSS;
				sa = samask ? (p & gra.AMASK) >> gra.ASHIFT << gra.ALOSS : 255;
				sa *= src_pbalpha;
				// Extract destination colors
				p = *(Uint32*)(dest_start + x*4 + y*dest_pitch);
				dr = (p & gra.RMASK) >> gra.RSHIFT << gra.RLOSS;
				dg = (p & gra.GMASK) >> gra.GSHIFT << gra.GLOSS;
				db = (p & gra.BMASK) >> gra.BSHIFT << gra.BLOSS;
				da = damask ? (p & gra.AMASK) >> gra.ASHIFT << gra.ALOSS : 255;
				// Calculate new values
				na = (255*(sa + 255*da) - sa*da); na += !na;
				nr = (dr*da*(255*255-sa) + sr*sa*255)/na;
				ng = (dg*da*(255*255-sa) + sg*sa*255)/na;
				nb = (db*da*(255*255-sa) + sb*sa*255)/na;
				na /= 255*255;
				// Apply the new values to the pixel
				p = (nr >> gra.RLOSS << gra.RSHIFT) |
					(ng >> gra.GLOSS << gra.GSHIFT) |
					(nb >> gra.BLOSS << gra.BSHIFT) |
					(na >> gra.ALOSS << gra.ASHIFT) ;
				*(Uint32*)(dest_start + x*4 + y*dest_pitch) = p;
			} // for (x = 0; x < w; x++)
		} // for (y = 0; y < h; y++)
		// Unlock both surfaces
		dest.unlock();
		unlock();
	}
}

void image::apply(Sint16 x, Sint16 y, SDL_Rect *src_part) {
	apply(gra.get_screen_buffer(), x, y, src_part);
}

void image::set_alpha(Uint8 a, bool enabled) {
	if (SDL_SetAlpha(_sdl_srf, (enabled ? SDL_SRCALPHA : 0), a+(a==127)) == -1)
		sdl_obj.error("Couldn't set alpha");
	alpha = a;
}

void image:: enable_alpha() {
	set_alpha(_sdl_srf->format->alpha, true );
}
void image::disable_alpha() {
	set_alpha(_sdl_srf->format->alpha, false);
}

void image::fill_rect(SDL_Rect *dstrect, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
	Uint32 pixel = SDL_MapRGBA(_sdl_srf->format, r, g, b, a);
	if(SDL_FillRect(_sdl_srf, dstrect, pixel)) sdl_obj.error("Couldn't fill rectangle in image");
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

image::image(string filename) : alpha(255) { // Constructor using a file name
	load(filename);
}

image::image(int w, int h) : alpha(255) {//, SDL_Color color) {
	generate_rect(w, h);//, color);
}

image::image(string text, font &text_font, SDL_Color text_color) : alpha(255) { // Constructor using a file name
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
	if (!_sdl_srf) sdl_obj.error("Couldn't initiate video mode");
}

void video_mode::flip() {
	if(SDL_Flip(_sdl_srf)) sdl_obj.error("Couldn't flip screen buffer");
}

video_mode::video_mode(int width, int height, int bpp, Uint32 flags) {
	init(width, height, bpp, flags);	
}
