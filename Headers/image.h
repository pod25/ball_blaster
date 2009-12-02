/*
 * File: level.h
 */
#ifndef IMAGE_H
#define IMAGE_H

class base_image {
protected:
	SDL_Surface* _sdl_srf;
public:
	bool empty();
	void apply(base_image dest, int x, int y, SDL_Rect *src_part = NULL);

	base_image() : _sdl_srf(NULL) {} // Default constructor
	~base_image() {} // Destructor
};

class image : public base_image {
public:
	bool load(string filename);
	void free();

	image() {} // Default constructor
	image(string filename); // Constructor using a file name
	~image(); // Destructor
};

class video_mode : public base_image {
public:
	bool init(int width, int height, int bpp, Uint32 flags);
	int  flip();

	video_mode() {} // Default constructor
	video_mode(int width, int height, int bpp, Uint32 flags);
};

#endif
/* end image.h */