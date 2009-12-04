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

void font::load(string file, int ptsize) {
	if (sdl_font) TTF_CloseFont(sdl_font);
	sdl_font = TTF_OpenFont(file.c_str(), ptsize);
	if (!sdl_font) throw exception(("Couldn't load font: " + file).c_str());
}

font::font(string file, int ptsize) : sdl_font(NULL) {load(file, ptsize);}

font::~font() {
	if (sdl_font) TTF_CloseFont(sdl_font);
}