/*
 * File: graphics.h
 */
#ifndef GRAPHICS_H
#define GRAPHICS_H

/*
 * graphics class
 */
class graphics {
	bool		refresh; //Whether the screen should be refreshed or not
	video_mode	screen_buffer; // The buffer that contains the screen image, SDL_Flip to flip
	image		background_buffer; // The buffer that contains the background image
	image		wall_buffer; // The buffer that contains the image for walls
	image		object_buffers[NUM_OBJECT_CLASSES]; // Every buffer for the images of the object types
	image		ball_buffer; // The buffer for the ball image
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