/*
 * File: graphics.h
 */
#ifndef GRAPHICS_H
#define GRAPHICS_H

/*
 * graphics class
 */
class graphics {
public:
	static const int SCREEN_WIDTH	= 1024;
	static const int SCREEN_HEIGHT	= 768;
	static const int SCREEN_BPP		= 32;
	static const int SCREEN_FLAGS	= SDL_SWSURFACE;; // | SDL_FULLSCREEN; // Creates a surface in system memory
	static const int IMAGE_FLAGS	= SDL_SWSURFACE | SDL_SRCALPHA; // Creates a surface in system memory with alpha-blending enabled
#if 0
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    static const Uint32 RMASK = 0xFF000000; static const Uint8 RSHIFT = 3*8, RLOSS = 0;
    static const Uint32 GMASK = 0x00FF0000; static const Uint8 GSHIFT = 2*8, GLOSS = 0;
    static const Uint32 BMASK = 0x0000FF00; static const Uint8 BSHIFT = 1*8, BLOSS = 0;
    static const Uint32 AMASK = 0x000000FF; static const Uint8 ASHIFT = 0*8, ALOSS = 0;
#else // Surfaces with these bit masks don't seem to ever be created
    static const Uint32 RMASK = 0x000000FF; static const Uint8 RSHIFT = 0*8, RLOSS = 0;
    static const Uint32 GMASK = 0x0000FF00; static const Uint8 GSHIFT = 1*8, GLOSS = 0;
    static const Uint32 BMASK = 0x00FF0000; static const Uint8 BSHIFT = 2*8, BLOSS = 0;
    static const Uint32 AMASK = 0xFF000000; static const Uint8 ASHIFT = 3*8, ALOSS = 0;
#endif
#else // These bit masks are used by SDL_DisplayFormatAlpha and TTF_RenderText_Blended
	static const Uint32 RMASK = 0x00FF0000; static const Uint8 RSHIFT = 2*8, RLOSS = 0;
	static const Uint32 GMASK = 0x0000FF00; static const Uint8 GSHIFT = 1*8, GLOSS = 0;
	static const Uint32 BMASK = 0x000000FF; static const Uint8 BSHIFT = 0*8, BLOSS = 0;
	static const Uint32 AMASK = 0xFF000000; static const Uint8 ASHIFT = 3*8, ALOSS = 0;
#endif
private:
	string	std_background_img;
	string	std_menu_background_img;
	string	std_ball_img;
	string	std_obj_images[NUM_OBJECT_CLASSES][NUM_DIRECTIONS];

	void init_sdl			();
	void init_images		(bool load_images = false);
	void init_object_images	(double scale = 1);

public:
	video_mode	screen_buffer;						// The buffer that contains the screen image, SDL_Flip to flip
	image		background_buffer;					// The buffer that contains the background image
	image		menu_background_buffer;				// The buffer that contains the menu background image
	image		object_layer_buffer;				// The buffer that contains the object layer
	image		ball_buffer;						// The buffer for the ball image
	image		object_buffers[NUM_OBJECT_CLASSES][NUM_DIRECTIONS];	// Every buffer for the images of the object types
	font		menu_font;
	SDL_Color	menu_color;
	SDL_Color	menu_color_selected;

	void update						();
	video_mode& get_screen_buffer	();
	void set_object_layer_size		(size_t w, size_t h);
	void set_grid_size				(uint grid_size);

	graphics();
private:
	graphics(const graphics&); //Protect from using copy constructor (until desired)
};

#endif
/* end graphics.h */