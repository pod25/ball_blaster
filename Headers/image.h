/*
 * File: level.h
 */
#ifndef IMAGE_H
#define IMAGE_H

/*
 * base_image class declaration
 */
class base_image {
protected:
	SDL_Surface* _sdl_srf;
public:
	bool			empty		();
	SDL_Surface*	get_sdl_srf	();
	int				get_w();
	int				get_h();
	// Access to variables
	void lock();
	void unlock();
	// Filling
	void fill_rect	(SDL_Rect *dstrect, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
	void set_color	(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);
	void clear		();
	// Shapes
	void line		(Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);

	base_image		() : _sdl_srf(NULL) {}	// Default constructor
	~base_image		() {}					// Destructor
private:
	base_image				(const base_image&) {} // Disable copy constructor
	base_image& operator=	(const base_image&) {} // Disable assignment
};

/*
 * image class declaration
 */
class image : public base_image {
	Uint8 alpha;
public:
	// Loading and freeing
	void load					(string filename);
	void generate_rect			(int w, int h); //, SDL_Color color = {0,0,0});
	void generate_text			(string text, font &text_font, SDL_Color text_color);
	void generate_resized		(base_image& src, double neww , double newh);
	void generate_rotated		(base_image& src, double angle, double zoom = 1);
	void generate_rotated_xy	(base_image& src, double angle, double zoomx, double zoomy);
	void resize					(double neww , double newh);
	void rotate					(double angle, double zoom = 1);
	void rotate_xy				(double angle, double zoomx, double zoomy);
	void free					();
	// Applying
	void apply					(base_image &dest, Sint16 x, Sint16 y, SDL_Rect *src_part = NULL);
	void apply					(Sint16 x, Sint16 y, SDL_Rect *src_part = NULL); // Apply on screen buffer
	// Alpha
	void set_alpha				(Uint8 a, bool enabled = true);
	void enable_alpha			();
	void disable_alpha			();

	image() : alpha(255) {}	// Default constructor
	image(int w, int h);	// SDL_Color color = {0,0,0});
	image(string filename);	// Constructor using a file name
	image(string text, font &text_font, SDL_Color text_color);
	image(base_image& src, string how, double p1);
	image(base_image& src, string how, double p1, double p2);
	image(base_image& src, string how, double p1, double p2, double p3);
	~image();				// Destructor
private:
	image			(const image&) {} // Disable copy constructor
	image& operator=(const image&) {} // Disable assignment
};

/*
 * video_mode class declaration
 */
class video_mode : public base_image {
public:
	void init(int width, int height, int bpp, Uint32 flags);
	void flip();

	video_mode() {} // Default constructor
	video_mode(int width, int height, int bpp, Uint32 flags);
private:
	video_mode				(const video_mode&) {} // Disable copy constructor
	video_mode& operator=	(const video_mode&) {} // Disable assignment
};

#endif
/* end image.h */