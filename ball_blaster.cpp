/*
 * File: ball_blaster.cpp
 */
#include "common.h"

/* 
 * main function
 */
int main(int argc, char* args[]) {

	// TODO: Temporary SDL implementation
	// TODO: Remove

	//The attributes of the screen
	const int SCREEN_WIDTH = 640;
	const int SCREEN_HEIGHT = 480;
	const int SCREEN_BPP = 32;

	//The surfaces that will be used
	sdl_surface* message	= NULL;
	sdl_surface* background	= NULL;
	sdl_surface* screen		= NULL;

	//The event structure that will be used
	SDL_Event event;

	//Initialize all SDL subsystems
	if(SDL_Init(SDL_INIT_EVERYTHING) == -1) return 1;

	//Set up the screen
	screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE);

	//If there was an error in setting up the screen
	if(screen == NULL) return 1;

	//Set the window caption
	SDL_WM_SetCaption("Hello World", NULL);

	//Load the images
	message = load_image("hello.bmp");
	background = load_image("background.bmp");

	//Apply the background to the screen
	apply_surface(0, 0, background, screen);
	apply_surface(320, 0, background, screen);
	apply_surface(0, 240, background, screen);
	apply_surface(320, 240, background, screen);

	//Apply the message to the screen
	apply_surface(180, 140, message, screen);

	//Update the screen
	if(SDL_Flip(screen) == -1) return 1;

	//Wait 2 seconds
	SDL_Delay(2000);

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
	SDL_FreeSurface(message);
	SDL_FreeSurface(background);
	
	//Quit SDL
	SDL_Quit();

	//Return
	return 0;
}