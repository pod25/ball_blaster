/*
 * File: game.cpp
 */
#include "common.h"

/*
 * Initialize game
 */
void game::init_game() {
	_quit			= false;
	cur_eh			= &menu_eh;
	set_window_pos	(0, 0);
}

/*
 * Game main function
 */
void game::main() {
	//The event structure that will be used
	SDL_Event event;
    
	// Graphic and logic timers
	uint last_refreshed = 0;
	uint last_stepped	= 0;
    last_refreshed		= SDL_GetTicks();
	last_stepped		= SDL_GetTicks();

	//While the user hasn't quit
	while(_quit == false) {
		//While there's an event to handle
		while(SDL_PollEvent(&event)) {
			//If the user has Xed out the window
			if(event.type == SDL_QUIT) {
				//Quit the program
				_quit = true;
			}
			// Informs event handler that an event has occured
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
		}

		// Step event handlers
		uint cur_time = SDL_GetTicks();
		if(cur_time - last_stepped >= 5) {
			//cur_eh->e_step(cur_time - last_stepped);
			cur_eh->e_step(5);
			last_stepped = cur_time;
		}

		// Handle frame rate
		cur_time = SDL_GetTicks();
		if(cur_time - last_refreshed >= 1000/FPS) {
			last_refreshed = cur_time;
			cur_eh->e_new_frame();
		}
	}
}

/*
 * Quit game
 */
void game::quit() {
	_quit = true;
}

/*
 * Position the window
 */
void game::set_window_pos(uint x, uint y) {
	_window_pos.x = x;
	_window_pos.y = y;
}
void game::set_window_pos(coords pos) {
	_window_pos.x = pos.x;
	_window_pos.y = pos.y;
}

/*
 * Get window position
 */
coords game::get_window_pos() {
	return _window_pos;
}

/*
 * Convert between level and window coordinates
 */
coords game::level_pos_from_window_pos(uint x, uint y) {
	return coords(_window_pos.x + x, _window_pos.y + y);
}

coords game::level_pos_from_window_pos(coords pos) {
	return coords(_window_pos.x + pos.x, _window_pos.y + pos.y);
}

coords game::window_pos_from_level_pos(uint x, uint y) {
	return coords(x - _window_pos.x, y - _window_pos.y);
}

coords game::window_pos_from_level_pos(coords pos) {
	return coords(pos.x - _window_pos.x, pos.y - _window_pos.y);
}
