/*
 * File: font.cpp
 */
#include "common.h"

bool font::empty() const {
	return !sdl_font;
}

TTF_Font* font::get_sdl_font() const {
	return sdl_font;
}

bool font::load(string file, int ptsize) {
	if (sdl_font) TTF_CloseFont(sdl_font);
	sdl_font = TTF_OpenFont(file.c_str(), ptsize);
	return !!sdl_font;
}

font::font(string file, int ptsize) : sdl_font(NULL) {
	if (!load(file, ptsize)) throw exception("Failed to load font");
}

font::~font() {
	if (sdl_font) TTF_CloseFont(sdl_font);
}