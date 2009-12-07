/*
 * File: level.h
 */
#ifndef IMAGE_H
#define IMAGE_H

class base_image {
protected:
	SDL_Surface* _sdl_srf;

	void lock();
	void unlock();
public:
	bool empty();
	void apply(base_image &dest, Sint16 x, Sint16 y, SDL_Rect *src_part = NULL);
	void apply(Sint16 x, Sint16 y, SDL_Rect *src_part = NULL); // Apply on screen buffer

	base_image() : _sdl_srf(NULL) {} // Default constructor
	~base_image() {} // Destructor
private:
	base_image(const base_image&) {} // Disable copy constructor
	base_image& operator=(const base_image&) {} // Disable assignment
};

class image : public base_image {
public:
	void load			(string filename);
	void generate_rect	(int w, int h);//, SDL_Color color = {0,0,0});
	void generate_text	(string text, font &text_font, SDL_Color text_color);
	void free			();
	void set_alpha		(Uint8 a, bool enabled = true);
	void enable_alpha	();
	void disable_alpha	();
	void fill_rect		(SDL_Rect *dstrect, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
	void set_color		(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);
	//void set_color		(SDL_Color color, Uint8 alpha = 255);
	void clear			();

	image() {} // Default constructor
	image(int w, int h);//, SDL_Color color = {0,0,0});
	image(string filename); // Constructor using a file name
	image(string text, font &text_font, SDL_Color text_color);
	~image(); // Destructor
private:
	image(const image&) {} // Disable copy constructor
	image& operator=(const image&) {} // Disable assignment
};

class video_mode : public base_image {
public:
	void init(int width, int height, int bpp, Uint32 flags);
	void flip();

	video_mode() {} // Default constructor
	video_mode(int width, int height, int bpp, Uint32 flags);
private:
	video_mode(const video_mode&) {} // Disable copy constructor
	video_mode& operator=(const video_mode&) {} // Disable assignment
};

#endif
/* end image.h */