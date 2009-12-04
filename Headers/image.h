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
	void apply(base_image &dest, int x, int y, SDL_Rect *src_part = NULL);
	void apply(int x, int y, SDL_Rect *src_part = NULL); // Apply on screen buffer

	base_image() : _sdl_srf(NULL) {} // Default constructor
	~base_image() {} // Destructor
private:
	base_image(const base_image&) {} // Disable copy constructor
	base_image& operator=(const base_image&) {} // Disable assignment
};

class image : public base_image {
public:
	bool load(string filename);
	bool load(string text, font &text_font, SDL_Color text_color);
	void free();

	image() {} // Default constructor
	image(string filename); // Constructor using a file name
	image(string text, font &text_font, SDL_Color text_color);
	~image(); // Destructor
private:
	image(const image&) {} // Disable copy constructor
	image& operator=(const image&) {} // Disable assignment
};

class video_mode : public base_image {
public:
	bool init(int width, int height, int bpp, Uint32 flags);
	int  flip();

	video_mode() {} // Default constructor
	video_mode(int width, int height, int bpp, Uint32 flags);
private:
	video_mode(const video_mode&) {} // Disable copy constructor
	video_mode& operator=(const video_mode&) {} // Disable assignment
};

#endif
/* end image.h */