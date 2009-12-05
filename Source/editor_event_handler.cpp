/*
 * File: editor_event_handler.cpp
 */
#include "common.h"

bool editor_event_handler::_is_const_type(uint oc) {
	switch(oc) {
		case OC_WALL:
		case OC_GOAL:
		case OC_CANNON:
			return true;
	}
	return false;
}

/*
 * Editor event handler constructor
 */
editor_event_handler::editor_event_handler() {
	_mouse_x			= 0;
	_mouse_y			= 0;
	_state				= STATE_DEFAULT;
	_can_edit_const		= false;
	_sel_obj_type		= OC_MAGNET;
	_objects_changed	= true;
	_scrolled			= true;
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
				_objects_changed = true;
				break;
		}
	}
	// Right mouse button
	else if(button == SDL_BUTTON_RIGHT) {
		// Which state?
		switch(_state) {
			case STATE_INSERTION:
				// Goto default mode
				_state = STATE_DEFAULT;
				break;
		}
	}
	// Mouse wheel up
	else if(button == SDL_BUTTON_WHEELUP) {
		switch(_state) {
			case STATE_DEFAULT:
				// Goto insertion mode
				_state = STATE_INSERTION;
				break;
			case STATE_INSERTION:
				_sel_obj_type++;
				if(_sel_obj_type >= NUM_OBJECT_CLASSES)
					_sel_obj_type = 0;
				if(!_can_edit_const) 
					while(_is_const_type(_sel_obj_type)) {
						_sel_obj_type++;
						if(_sel_obj_type >= NUM_OBJECT_CLASSES)
							_sel_obj_type = 0;
					}
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
	coords pos = gam.get_window_pos();
	if(key == SDLK_UP)
		pos.y -= 5;
	if(key == SDLK_DOWN)
		pos.y += 5;
	if(key == SDLK_LEFT)
		pos.x -= 5;
	if(key == SDLK_RIGHT)
		pos.x += 5;
	if(pos.x < 0) pos.x = 0;
	if(pos.y < 0) pos.y = 0;
	gam.set_window_pos(pos);
	_scrolled = true;
}

/*
 * Editor new frame handler
 */
void editor_event_handler::e_new_frame() {
	// Update object layer?
	if(_objects_changed || _scrolled) {
		gra.object_layer_buffer.clear();

		coords start_square = lev.vector_coords_from_pixel(gam.level_pos_from_window_pos(0, 0));
		coords cur_square = start_square;

		while(gam.window_pos_from_level_pos(lev.pixel_coords_from_vector(cur_square.x, cur_square.y)).x < gra.SCREEN_WIDTH) {
			cur_square.y = start_square.y;
			while(gam.window_pos_from_level_pos(lev.pixel_coords_from_vector(cur_square.x, cur_square.y)).y < gra.SCREEN_HEIGHT) {
				coords object_window_pos = gam.window_pos_from_level_pos(lev.pixel_coords_from_vector(cur_square.x, cur_square.y));
				int num_objects = lev.num_objects(cur_square.x, cur_square.y);
				for(int i = 0; i < num_objects ; i++) {
					object*				cur_o		= lev.get_object(cur_square.x, cur_square.y, i);
					wall*				wall_o		= dynamic_cast<wall*>(cur_o);
					goal*				goal_o		= dynamic_cast<goal*>(cur_o);
					cannon*				cannon_o	= dynamic_cast<cannon*>(cur_o);
					magnet*				magnet_o	= dynamic_cast<magnet*>(cur_o);
					fan*				fan_o		= dynamic_cast<fan*>(cur_o);
					directed_object*	dir_o		= dynamic_cast<directed_object*>(cur_o);
					nondirected_object*	nondir_o	= dynamic_cast<nondirected_object*>(cur_o);

					image* image_buffer_array;
					image* image_buffer_ptr;

					if(wall_o)
						image_buffer_array = gra.object_buffers[OC_WALL];
					else if(goal_o)
						image_buffer_array = gra.object_buffers[OC_GOAL];
					else if(cannon_o)
						image_buffer_array = gra.object_buffers[OC_CANNON];
					else if(magnet_o)
						image_buffer_array = gra.object_buffers[OC_MAGNET];
					else if(fan_o)
						image_buffer_array = gra.object_buffers[OC_FAN];

					if(dir_o)
						image_buffer_ptr = &image_buffer_array[dir_o->get_dir()];
					else if(nondir_o)
						image_buffer_ptr = &image_buffer_array[DIR_NODIR];

					image_buffer_ptr->apply(gra.object_layer_buffer, object_window_pos.x, object_window_pos.y);
				}
				cur_square.y++;
			}
			cur_square.x++;
		}
	}

	gra.background_buffer.apply(0, 0);
	gra.object_layer_buffer.apply(0, 0);
	gra.update();

	_objects_changed = false;
	_scrolled = false;
}

/*
 * Editor step handler
 */
void editor_event_handler::e_step(int delta_t) {

}