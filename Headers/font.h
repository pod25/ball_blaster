/*
 * File: level.h
 */
#ifndef FONT_H
#define FONT_H

class font {
protected:
	TTF_Font* sdl_font;
public:
	bool empty() const;
	TTF_Font* get_sdl_font() const;
	bool load(string file, int ptsize);

	font()                        : sdl_font(NULL) {} // Default constructor
	font(string file, int ptsize);
	~font(); // Destructor
private:
	font(const font&) {} // Disable copy constructor
	font& operator=(const font&) {} // Disable assignment
};

#endif
/* end font.h */