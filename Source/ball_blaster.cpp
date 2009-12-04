/*
 * File: ball_blaster.cpp
 */
#include "common.h"

/*
 * Global (!) variables (extern in common.h)
 */
editor_event_handler	editor_eh;	// Editor event handler
simulator_event_handler	sim_eh;		// Simulator event handler
event_handler*			cur_eh;		// Pointer to current event handler
level					lev;		// Level
sdl_handler				sdl_obj;	// SDL handler
graphics				gra;		// Graphics
physics					phy;		// Physics

/* 
 * main function
 */
int main(int argc, char* args[]) {
	try{
		// TODO: Temporary SDL implementation
		// TODO: Remove

		//The attributes of the screen
		const int SCREEN_WIDTH = 640;
		const int SCREEN_HEIGHT = 480;
		const int SCREEN_BPP = 32;

		//The font that's going to be used
		font lazy_font("Fonts/lazy.ttf", 28);
		//The color of the font
		SDL_Color text_color = {0, 0, 0}; 

		//The surfaces that will be used
		//video_mode	screen(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE);
		image hello("temp/hello.png");
		image message("The quick brown fox jumps over the lazy dog", lazy_font, text_color);
		image rect_srf;
		image background("temp/background.png");
		//Render the text

		//The event structure that will be used
		SDL_Event event;

		//Apply the background to the screen
		background.apply(0, 0);
		background.apply(320, 0);
		background.apply(0, 240);
		background.apply(320, 240);

		//Apply the images to the screen
		hello.set_alpha(64);
		hello.apply(180, 140);
		message.apply(10, 160);

		rect_srf.load("wall/Earth SMWar.png");
		//rect_srf.generate_text("\"The quick brown fox jumps over the lazy dog\"", lazy_font, text_color);
		rect_srf.disable_alpha ();
		rect_srf.enable_alpha ();
		rect_srf.set_alpha(128);
		SDL_Color color; color.r = 128; color.g = 0; color.b = 255;
		//rect_srf.set_color(color);
		//rect_srf.set_color(color.r, color.g, color.b);
		rect_srf.apply(5, 40);

		//Update the screen
		gra.set_refresh_flag();
		gra.update();

		/* LEVEL TEST */
		lev.load_level("in");
		lev.save_level("out");

		//While the user hasn't quit
		bool quit = false;
		while(quit == false) {
			//While there's an event to handle
			while(SDL_PollEvent(&event)) {
				//If the user has Xed out the window
				if(event.type == SDL_QUIT) {
					//Quit the program
					quit = true;
				}
			}
		}

		//Free the surfaces
		//message.free();
		//background.free();
	}
	catch (exception &e) {
		cerr << "Unexpected error occured in main()" << endl;
		cerr << "Caught: " << e.what() << endl;
		cerr << "Type: " << typeid(e).name() << endl;
		MessageBox(0, e.what(), "main(): An unexpected error has occured", MB_ICONERROR);
	};
	//Return
	return 0;
}