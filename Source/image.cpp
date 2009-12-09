/*
 * File: image.cpp
 */
#include "common.h"

/*************************************************
 * base_image class methods
 *************************************************/ 
/*
 * Lock / Unlock surfaces
 */
void base_image::lock() {
	if(SDL_MUSTLOCK(_sdl_srf) && SDL_LockSurface(_sdl_srf) == -1)
		throw exception("Couldn't lock image");
}
void base_image::unlock() {
	if(SDL_MUSTLOCK(_sdl_srf))
		SDL_UnlockSurface(_sdl_srf);
}

/*
 * Has surface?
 */
bool base_image::empty() {
	return !_sdl_srf;
}

/*
 * Get surface?
 */
SDL_Surface* base_image::get_sdl_srf() {
	return _sdl_srf;
}

/*
 * Fill rectangle with RGBA color
 */
void base_image::fill_rect(SDL_Rect *dstrect, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
	Uint32 pixel = SDL_MapRGBA(_sdl_srf->format, r, g, b, a);
	if(SDL_FillRect(_sdl_srf, dstrect, pixel))
		sdl_obj.error("Couldn't fill rectangle in image");
}

/*
 * Clear the surface with RGBA color
 */
void base_image::set_color(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
	fill_rect(NULL, r, g, b, a);
}
/*
 * Clear buffer (to total transparency)
 */
void base_image::clear() {
	set_color(0, 0, 0, 0);
}

/*
 * Draw line
 */
void base_image::line(Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
	if(aalineRGBA(_sdl_srf, x1, y1, x2, y2, r, g, b, a))
		sdl_obj.error("Couldn't draw line");
}

/*************************************************
 * image class methods
 *************************************************/ 
/*
 * Load image
 */
void image::load(string filename) {
	filename = "Images/" + filename;
	// Free old image if any
	if(_sdl_srf)
		SDL_FreeSurface(_sdl_srf);
	// Load the image
	SDL_Surface* loaded_image = IMG_Load(filename.c_str());
	if(loaded_image != NULL) {
		// Create an optimized image
		_sdl_srf = SDL_DisplayFormatAlpha(loaded_image);
		// Free the old image
		SDL_FreeSurface(loaded_image);
		if(!_sdl_srf)
			sdl_obj.error("Couldn't optimize loaded image");
	}
	else
		sdl_obj.error("Couldn't load image: " + filename);
}

/*
 * Create a new surface of given size
 */
void image::generate_rect(int w, int h) {//, SDL_Color color) {
	// Free old image if any
	if(_sdl_srf)
		SDL_FreeSurface(_sdl_srf);
	_sdl_srf = SDL_CreateRGBSurface(gra.IMAGE_FLAGS, w, h, gra.SCREEN_BPP, gra.RMASK, gra.GMASK, gra.BMASK, gra.AMASK);
	//if (color.r != 0 || color.g != 0 || color.b != 0) set_color(color);
	if(!_sdl_srf)
		sdl_obj.error("Couldn't generate rectangular image");
}

/*
 * Generate a surface with text
 */
void image::generate_text(string text, font &text_font, SDL_Color text_color) {
	// Free old image if any
	if(_sdl_srf)
		SDL_FreeSurface(_sdl_srf);
	_sdl_srf = TTF_RenderText_Blended(text_font.get_sdl_font(), text.c_str(), text_color);
	if(!_sdl_srf)
		sdl_obj.error("Couldn't generate image from text");
}

/*
 * Resize surface
 */
void image::generate_resized(base_image& src, double zoomx, double zoomy) {
	// Free old image if any
	SDL_Surface* temp_srf = zoomSurface(src.get_sdl_srf(), zoomx, zoomy, 1);
	if(_sdl_srf)
		SDL_FreeSurface(_sdl_srf);
	_sdl_srf = temp_srf;
	if(!_sdl_srf)
		sdl_obj.error("Couldn't generate xy-rotated image");
}

/*
 * Rotate surface
 */
void image::generate_rotated (base_image& src, double angle, double zoom) {
	// Free old image if any
	SDL_Surface* temp_srf = rotozoomSurface(src.get_sdl_srf(), angle, zoom, 1);
	if(_sdl_srf)
		SDL_FreeSurface(_sdl_srf);
	_sdl_srf = temp_srf;
	if(!_sdl_srf)
		sdl_obj.error("Couldn't generate rotated image");
}

/*
 * Rotate and scale surface
 */
void image::generate_rotated_xy (base_image& src, double angle, double zoomx, double zoomy) {
	// Free old image if any
	SDL_Surface* temp_srf = rotozoomSurfaceXY(src.get_sdl_srf(), angle, zoomx, zoomy, 1);
	if(_sdl_srf)
		SDL_FreeSurface(_sdl_srf);
	_sdl_srf = temp_srf;
	if(!_sdl_srf)
		sdl_obj.error("Couldn't generate xy-rotated image");
}

void image::resize   (              double zoomx, double zoomy) {generate_resized   (*this,        zoomx, zoomy);}
void image::rotate   (double angle, double zoom               ) {generate_rotated   (*this, angle, zoom        );}
void image::rotate_xy(double angle, double zoomx, double zoomy) {generate_rotated_xy(*this, angle, zoomx, zoomy);}

/*
 * Free surface
 */
void image::free() {
	if(_sdl_srf)
		SDL_FreeSurface(_sdl_srf);
	else
		throw exception("Trying to free image that is not loaded");
	_sdl_srf = NULL;
}

/*
 * Apply surface to another
 */
void image::apply(base_image &dest, Sint16 x, Sint16 y, SDL_Rect *src_part) { // Can't check alpha value of dest so it's ignored
	SDL_Surface* dest_srf = dest.get_sdl_srf();
	if(!_sdl_srf)
		throw invalid_argument("Image being applied not loaded");
	if(!dest_srf)
		throw invalid_argument("Image being applied on not loaded");

	// Make a temporary rectangle to hold the offsets
	SDL_Rect offset;
	offset.x = x;
	offset.y = y;

	// Check which of the algorithms should be used
	if (!(_sdl_srf->flags & SDL_SRCALPHA && (alpha < 255 || _sdl_srf->format->Amask &&
		  dest_srf->flags & SDL_SRCALPHA && dest_srf->format->Amask)) ) { 
		// Blit the surface using SDL's build in blit function
		if (SDL_BlitSurface(_sdl_srf, src_part, dest_srf, &offset) < 0)
			sdl_obj.error("Couldn't blit image");
	}
	else { // Blit the surface by accessing the pixel data
		// Control color depth
		if (_sdl_srf->format->BitsPerPixel != 32) throw invalid_argument("Source image has invalid pixel format");
		if (dest_srf->format->BitsPerPixel != 32) throw invalid_argument("Destination image has invalid pixel format");
		// Constrol the masks of the surfaces
		Uint32 samask =                                  _sdl_srf->format->Amask;
		Uint32 damask = dest_srf->flags & SDL_SRCALPHA ? dest_srf->format->Amask : 0;
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

/*
 * Set per-surface alpha value
 */
void image::set_alpha(Uint8 a, bool enabled) {
	if (SDL_SetAlpha(_sdl_srf, (enabled ? SDL_SRCALPHA : 0), a+(a==127)) == -1)
		sdl_obj.error("Couldn't set alpha");
	alpha = a;
}

/*
 * Enable / Disable alpha for surface
 */
void image:: enable_alpha() {
	set_alpha(_sdl_srf->format->alpha, true );
}
void image::disable_alpha() {
	set_alpha(_sdl_srf->format->alpha, false);
}

/*
 * Constructors
 */
image::image(string filename) : alpha(255) { // Constructor using a file name
	load(filename);
}
image::image(int w, int h) : alpha(255) {//, SDL_Color color) {
	generate_rect(w, h);//, color);
}
image::image(string text, font &text_font, SDL_Color text_color) : alpha(255) { // Constructor using a file name
	generate_text(text, text_font, text_color);
}
image::image(base_image& src, string how, double p1) : alpha(255) {
	if      (how == "rot" ) generate_rotated(src, p1);
	else if (how == "zoom") generate_resized(src, p1, p1);
	else                    throw invalid_argument("Can't " + how + " image using 1 argument");
}
image::image(base_image& src, string how, double p1, double p2) : alpha(255) {
	if      (how == "rot" ) generate_rotated(src, p1, p2);
	else if (how == "zoom") generate_resized(src, p1, p2);
	else                    throw invalid_argument("Can't " + how + " image using 2 arguments");
}
image::image(base_image& src, string how, double p1, double p2, double p3) : alpha(255) {
	if      (how == "rot-xy") generate_rotated_xy(src, p1, p2, p3);
	else                      throw invalid_argument("Can't " + how + " image using 3 arguments");
}

/*
 * Destructor
 */
image::~image() {
	if(_sdl_srf)
		SDL_FreeSurface(_sdl_srf);
}

/*************************************************
 * video_mode class methods
 *************************************************/ 
/*
 * Initialize video mode
 */
void video_mode::init(int width, int height, int bpp, Uint32 flags) {
	if (_sdl_srf) throw exception("Video mode already initiated");
	_sdl_srf = SDL_SetVideoMode(width, height, bpp, flags);
	if (!_sdl_srf) sdl_obj.error("Couldn't initiate video mode");
}

/*
 * Output to screen
 */
void video_mode::flip() {
	if(SDL_Flip(_sdl_srf))
		sdl_obj.error("Couldn't flip screen buffer");
}
/*
 * Constructor
 */
video_mode::video_mode(int width, int height, int bpp, Uint32 flags) {
	init(width, height, bpp, flags);
}
