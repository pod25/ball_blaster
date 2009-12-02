/*
 * File: graphics.h
 */
#ifndef GRAPHICS_H
#define GRAPHICS_H

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

	bool		refresh; //Whether the screen should be refreshed or not
	video_mode	screen_buffer; // The buffer that contains the screen image, SDL_Flip to flip
	image		background_buffer; // The buffer that contains the background image
	image		ball_buffer; // The buffer for the ball image
	image		object_buffers[NUM_OBJECT_CLASSES]; // Every buffer for the images of the object types

	void init_images(bool load_images = false);
public:
	void set_refresh_flag();
	//void init_graphics(); // Handeled by constructor ???
	void update(); // Does ???

	graphics();
private:
	graphics(const graphics&); //Protect from using copy constructor (until desired)
};

#endif
/* end graphics.h */