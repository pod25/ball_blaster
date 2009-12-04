/*
 * File: game.cpp
 */
#include "common.h"

/*************************************************
 * PUBLIC FUNCTION DEFINITIONS 
 *************************************************/
/*
 * Initialize game
 */
void game::init_game() {

}

/*
 * 
 */
void game::main() {
	//The event structure that will be used
	SDL_Event event;
    
	uint last_refreshed = 0;
	//Start the timer
    last_refreshed = SDL_GetTicks();

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
			// Informs event handler that an action has occured
			else if(event.type == SDL_KEYDOWN) {
				cur_eh->e_key_down(event.key.keysym.sym);
			}
			else if(event.type == SDL_KEYUP) {
				cur_eh->e_key_up(event.key.keysym.sym);
			}
			else if(event.type == SDL_MOUSEMOTION) {
				cur_eh->e_mouse_move(event.button.x, event.button.y);
			}
			else if(event.type == SDL_MOUSEBUTTONDOWN) {
				cur_eh->e_mouse_down(event.button.x, event.button.y, event.button.button);
			}			
			else if(event.type == SDL_MOUSEBUTTONUP) {
				cur_eh->e_mouse_up(event.button.x, event.button.y, event.button.button);
			}
			
			uint cur_time = SDL_GetTicks();
			if(cur_time-last_refreshed >= 1000/FPS) {
				last_refreshed = cur_time;
				cur_eh->e_new_frame();
			}
		}
	}
}

