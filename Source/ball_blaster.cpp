/*
 * File: ball_blaster.cpp
 */
#include "common.h"

/*
 * Global (!) variables (extern in common.h)
 */
error_handler			err_hndl_obj;	// Error handler
editor_event_handler	editor_eh;		// Editor event handler
menu_event_handler		menu_eh;		// Menu event handler
simulator_event_handler	sim_eh;			// Simulator event handler
event_handler*			cur_eh;			// Pointer to current event handler
game					gam;			// Game
sdl_handler				sdl_obj;		// SDL handler
graphics				gra;			// Graphics
level					lev;			// Level
physics					phy;			// Physics

/* 
 * Main function
 */
int main(int argc, char* args[]) {
	try{
		// Initialize game
		gam.init_game();

		// Go to main loop
		gam.main();
	}
	catch (exception &e) {err_hndl_obj.inform_about_error("main()", e);};

	// Quit application
	return 0;
}