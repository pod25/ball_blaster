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

		//The surfaces that will be used
		video_mode	screen(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE);
		image		message("images/temp/hello.png");
		image		background("images/temp/background.png");

		//The event structure that will be used
		SDL_Event event;

		//Initialize all SDL subsystems
		if(SDL_Init(SDL_INIT_EVERYTHING) == -1) return 1;

		//Set up the screen
		//screen.init(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE);

		//If there was an error in setting up the screen
		if(screen.empty()) return 1;

		//Set the window caption
		SDL_WM_SetCaption("Hello World", NULL);

		//Load the images
		//message.load("images/temp/hello.png");
		//background.load("images/temp/background.png");

		//Apply the background to the screen
		background.apply(screen, 0, 0);
		background.apply(screen, 320, 0);
		background.apply(screen, 0, 240);
		background.apply(screen, 320, 240);

		//Apply the message to the screen
		message.apply(screen, 180, 140);

		//Update the screen
		if((screen.flip()) == -1) return 1;

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

		//Quit SDL
		SDL_Quit();
	}
	catch ( exception &e ) {
		cerr << "Unexpected error occured" << endl;
		cerr << "Caught: " << e.what( ) << endl;
		cerr << "Type: " << typeid( e ).name( ) << endl;
	};
	//Return
	return 0;
}