/*
 * File: graphics.h
 */
#ifndef GRAPHICS_H
#define GRAPHICS_H

/*
 * sdl_handler class
 */
class sdl_handler {
	void init() {
		//Initialize all SDL subsystems
		if(SDL_Init(SDL_INIT_EVERYTHING) == -1) throw exception("SDL failed to initiate");
		//Set the window caption
		SDL_WM_SetCaption("Ball Blaster 2000", NULL);
		//Initialize SDL_ttf
		if(TTF_Init() == -1) throw exception("TTF failed to initiate");
	}
public:

	sdl_handler(Uint32 sdl_flags = SDL_INIT_EVERYTHING) { // Embedded default constructor
		init();
	}
	~sdl_handler() { // Destructor
		//Quit SDL_ttf
		TTF_Quit(); 
		//Quit SDL
		SDL_Quit();
	}
};
	

/*
 * graphics class
 */
class graphics {
	static const int SCREEN_WIDTH	= 640;
	static const int SCREEN_HEIGHT	= 480;
	static const int SCREEN_BPP		= 32;
	static const int SCREEN_FLAGS	= SDL_SWSURFACE;
	string std_background_img;
	string std_ball_img;
	string std_obj_images[NUM_OBJECT_CLASSES];

	sdl_handler sdl_object;
	bool		refresh; //Whether the screen should be refreshed or not
	video_mode	screen_buffer; // The buffer that contains the screen image, SDL_Flip to flip
	image		background_buffer; // The buffer that contains the background image
	image		ball_buffer; // The buffer for the ball image
	image		object_buffers[NUM_OBJECT_CLASSES]; // Every buffer for the images of the object types

	void init_sdl();
	void init_images(bool load_images = false);

public:
	void set_refresh_flag();
	//void init_graphics(); // Handeled by constructor ???
	void update(); // Does ???
	video_mode& get_screen_buffer();

	graphics();
private:
	graphics(const graphics&); //Protect from using copy constructor (until desired)
};

#endif
/* end graphics.h */