/*
 * File: sdl_handler.h
 */
#ifndef SDL_HANDLER_H
#define SDL_HANDLER_H

/*
 * sdl_handler class declaration
 */
class sdl_handler {
	bool sdl_initiated;
	bool ttf_initiated;
	void init() {
		//Initialize all SDL subsystems
		if(SDL_Init(SDL_INIT_EVERYTHING) == -1)
			throw exception("SDL failed to initiate");
		else
			sdl_initiated = true;
		//Set the window caption
		SDL_WM_SetCaption("Ball Blaster 3000", NULL);
		//Initialize SDL_ttf
		if(TTF_Init() == -1)
			throw exception("TTF failed to initiate");
		else
			ttf_initiated = true;
	}
public:
	void error(string message) {
		string err_str;
		err_str = err_str + "SDL error: \'" + SDL_GetError() + "\' Comment: \'" + message + '\'';
		throw exception(err_str.c_str());
	}
	sdl_handler(Uint32 sdl_flags = SDL_INIT_EVERYTHING)
		: sdl_initiated(false), ttf_initiated(false) { // Embedded default constructor
		init();
	}
	~sdl_handler() { // Destructor
		//Quit SDL_ttf
		if(ttf_initiated)
			TTF_Quit(); 
		//Quit SDL
		if(sdl_initiated)
			SDL_Quit();
	}
};

#endif
/* end sdl_handler.h */