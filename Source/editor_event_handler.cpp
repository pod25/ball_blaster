/*
 * File: editor_event_handler.cpp
 */
#include "common.h"

/*
 * Editor event handler constructor
 */
editor_event_handler::editor_event_handler() {
	_mouse_x		= 0;
	_mouse_y		= 0;
	_state			= STATE_DEFAULT;
	_can_edit_const	= false;
	_sel_obj_type	= OC_MAGNET;
}

/*
 * Editor mouse movement handler
 */
void editor_event_handler::e_mouse_move(int mouse_x, int mouse_y) {
	_mouse_x = mouse_x;
	_mouse_y = mouse_y;
}

/*
 * Editor mouse button handlers
 */
void editor_event_handler::e_mouse_down(int mouse_x, int mouse_y, int button) {

}
void editor_event_handler::e_mouse_up(int mouse_x, int mouse_y, int button) {
	// Left mouse button
	if(button == SDL_BUTTON_LEFT) {
		// Which state?
		switch(_state) {
			case STATE_DEFAULT:
				// Goto insertion mode
				_state = STATE_INSERTION;
				break;
			case STATE_INSERTION:
				// Insert object at position
				coords level_pos = gam.level_pos_from_window_pos(mouse_x, mouse_y);
				lev.insert_obj_at_pixel(_sel_obj_type, level_pos.x, level_pos.y, _can_edit_const);
				break;
		}
	}
}

/*
 * Editor key button handlers
 */
void editor_event_handler::e_key_down(int key) {

}
void editor_event_handler::e_key_up(int key) {

}

/*
 * Editor new frame handler
 */
void editor_event_handler::e_new_frame() {

}

/*
 * Editor step handler
 */
void editor_event_handler::e_step(int delta_t) {

}