/*
 * File: editor_event_handler.cpp
 */
#include "common.h"

/*
 * Is object of constant type?
 */
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
 * Is object directed?
 */
bool editor_event_handler::_is_dir_type(uint oc) {
	switch(oc) {
		case OC_MAGNET:
		case OC_FAN:
			return true;
	}
	return false;
}

/*
 * Reset editor
 */
void editor_event_handler::reset_state() {
	_state				= STATE_DEFAULT;
	_left_mouse_is_down	= false;
	_right_mouse_is_down= false;
	_sel_obj_type		= OC_MAGNET;
	_floating_window_pos.x = 0;
	_floating_window_pos.y = 0;
}


/*
 * Editor event handler constructor
 */
editor_event_handler::editor_event_handler() {
	_mouse_x			= 0;
	_mouse_y			= 0;
	_left_mouse_is_down	= false;
	_right_mouse_is_down= false;
	_state				= STATE_DEFAULT;
	_can_edit_const		= false;
	_sel_obj_type		= OC_MAGNET;
	_floating_window_pos.x = 0;
	_floating_window_pos.y = 0;
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
	if(button == SDL_BUTTON_LEFT)
		_left_mouse_is_down = true;
	if(button == SDL_BUTTON_RIGHT)
		_right_mouse_is_down = true;

	// Hold left mouse button control cannon
	if(button == SDL_BUTTON_LEFT && lev.cannon_exists()) {
		coords mouse_vector(lev.vector_coords_from_pixel(gam.level_pos_from_window_pos(_mouse_coords())));
		if(lev.cannon_coords().x == mouse_vector.x && lev.cannon_coords().y == mouse_vector.y) {
			switch(_state) {
				case STATE_DEFAULT:
				case STATE_INSERTION:
					// Goto cannon config mode
					_state = STATE_CANNON_CONFIG;
					break;
			}
		}
	}
}
void editor_event_handler::e_mouse_up(int mouse_x, int mouse_y, int button) {
	if(button == SDL_BUTTON_LEFT)
		_left_mouse_is_down = false;
	if(button == SDL_BUTTON_RIGHT)
		_right_mouse_is_down = false;

	// Left mouse button
	if(button == SDL_BUTTON_LEFT) {
		coords cannon_corner = lev.pixel_coords_from_vector(coords(lev.cannon_coords()));
		coords cannon_center = cannon_corner + coords(1,1) * (lev.get_grid_size()/2);
		coords conv_cannon_c = gam.window_pos_from_level_pos(cannon_center);
		vec    cannon_dir    = _mouse_coords() - conv_cannon_c;

		// Which state?
		switch(_state) {
			case STATE_CANNON_CONFIG:
				dynamic_cast<cannon*>(lev.get_object(lev.cannon_coords()))->_shot_vec =
					negated_y(cannon_dir/lev.get_pixels_per_le());
				_state = STATE_DEFAULT;
				break;
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
			case STATE_DEFAULT:
				/* Delete object at cursor */ {
					coords del_pos = gam.level_pos_from_window_pos(mouse_x, mouse_y);
					lev.remove_obj_at_pixel(del_pos.x, del_pos.y, _can_edit_const);
				}
				_objects_changed = true;
				break;
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
				// Change object selection
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
	// Mouse wheel down
	else if(button == SDL_BUTTON_WHEELDOWN) {
				switch(_state) {
			case STATE_DEFAULT:
				// Goto insertion mode
				_state = STATE_INSERTION;
				break;
			case STATE_INSERTION:
				// Change object selection
				_sel_obj_type--;
				if(_sel_obj_type < 0)
					_sel_obj_type = NUM_OBJECT_CLASSES - 1;
				if(!_can_edit_const) 
					while(_is_const_type(_sel_obj_type)) {
						_sel_obj_type--;
						if(_sel_obj_type < 0)
							_sel_obj_type = NUM_OBJECT_CLASSES - 1;
					}
				break;
		}
	}
}

/*
 * Editor key button handlers
 */
void editor_event_handler::e_key_down(int key) {
	// Start simulation
	if(key == SDLK_RETURN)
		start_simulation();
	// Go back to menu
	else if(key == SDLK_ESCAPE){
		//menu_eh.menu_reset();
		if (_can_edit_const) menu_eh.reset_current_state(); // Editor could have created a new level; display this in that case
		cur_eh = &menu_eh;
		reset_state();
	}
	// Save level when in level editor
	else if(key == SDLK_s && _can_edit_const)
		lev.save_level("");
	else if(key == SDLK_KP_MINUS)
		lev.set_grid_size(lev.get_grid_size() - 4);
	else if(key == SDLK_KP_PLUS)
		lev.set_grid_size(lev.get_grid_size() + 4);
		
}
void editor_event_handler::e_key_up(int key) {
}

/*
 * Editor new frame handler
 */
void editor_event_handler::e_new_frame() {
	// Apply background to screen
	gra.background_buffer.apply(0, 0);

	// Apply object layer
	SDL_Rect src_rect;
	src_rect.x = gam.get_window_pos().x;
	src_rect.y = gam.get_window_pos().y;
	src_rect.w = gra.SCREEN_WIDTH;
	src_rect.h = gra.SCREEN_HEIGHT;
	gra.object_layer_buffer.apply(0, 0, &src_rect);

	// Show number of available objects
	image text_bg;
	text_bg.generate_rect(380, 35);
	text_bg.set_color(255, 255, 255, 125);
	text_bg.apply(0, 0);
	image available_objects;
	string fans(to_string(lev.get_fans_left()));
	string magnets(to_string(lev.get_magnets_left()));
	string out = string("Fans: ")+fans+string("	  Magnets: ")+magnets;
	available_objects.generate_text(out, gra.menu_font, gra.menu_color);
	available_objects.apply(5, 5);

	// Show cannon vector
	coords cannon_corner = lev.pixel_coords_from_vector(lev.cannon_coords().x, lev.cannon_coords().y);
	coords cannon_center = cannon_corner + coords(1,1) * (lev.get_grid_size()/2);
	coords conv_cannon_c = gam.window_pos_from_level_pos(cannon_center);
	if(lev.cannon_exists() && _state != STATE_CANNON_CONFIG) {
		vec line_vec = negated_y(lev.get_cannon()->_shot_vec);
		line_vec *= lev.get_pixels_per_le();
		gra.screen_buffer.line(conv_cannon_c.x, conv_cannon_c.y,
			conv_cannon_c.x + Sint16(line_vec.x),
			conv_cannon_c.y + Sint16(line_vec.y),
			100, 10, 0);
	}
		
	switch(_state) {
		case STATE_INSERTION:
			/* Plot object placement preview */ {
				image*	image_buffer_array	= gra.object_buffers[_sel_obj_type];
				image*	image_buffer_ptr;
				coords	mouse_level_pos		= gam.level_pos_from_window_pos(_mouse_x, _mouse_y);
				coords	square_vector		= lev.vector_coords_from_pixel(mouse_level_pos.x, mouse_level_pos.y);
				coords	square_pos			= gam.window_pos_from_level_pos(lev.pixel_coords_from_vector(square_vector));
				bool	directed			= _is_dir_type(_sel_obj_type);
				uint	dir					= lev.dir_from_pixel(mouse_level_pos.x, mouse_level_pos.y);

				if(directed)
					image_buffer_ptr = &image_buffer_array[dir]; 
				else
					image_buffer_ptr = &image_buffer_array[DIR_NODIR];
				if(lev.can_insert_obj(square_vector.x, square_vector.y, directed, dir))
					image_buffer_ptr->set_alpha(160, true);
				else
					image_buffer_ptr->set_alpha(64, true);
				image_buffer_ptr->apply(square_pos.x, square_pos.y);
				image_buffer_ptr->set_alpha(SDL_ALPHA_OPAQUE, true);
			}
			break;
		case STATE_CANNON_CONFIG:
			// Follow mouse with cannon vector
			gra.screen_buffer.line	(conv_cannon_c.x, conv_cannon_c.y,
									 _mouse_x, _mouse_y, 100, 10, 0);
			break;
	}

	// Refresh screen
	gra.update();
}

/*
 * Refresh object layer when objects have changed
 */
void editor_event_handler::objects_changed(size_t x, size_t y, bool all) {
	// Redraw all squares
	if(all) {
		// Clear whole buffer
		gra.object_layer_buffer.clear();

		for(size_t cur_x = 0; cur_x < lev.get_width(); cur_x++) {
			for(size_t cur_y = 0; cur_y < lev.get_height(); cur_y++) {
				_plot_square(cur_x, cur_y);
			}
		}
	}
	// Redraw one square
	else {
		// Clear square
		coords object_level_pos = lev.pixel_coords_from_vector(x, y);
		SDL_Rect clear_rect;
		clear_rect.x = object_level_pos.x;
		clear_rect.y = object_level_pos.y;
		clear_rect.w = lev.get_grid_size();
		clear_rect.h = lev.get_grid_size();
		gra.object_layer_buffer.fill_rect(&clear_rect, 0, 0, 0, SDL_ALPHA_TRANSPARENT);

		_plot_square(x, y);
	}
}

/*
 * Output one square to object layer
 */
 void editor_event_handler::_plot_square(size_t x, size_t y) {
	coords object_level_pos = lev.pixel_coords_from_vector(x, y);
	int num_objects = lev.num_objects(x, y);
	for(int i = 0; i < num_objects ; i++) {
		object*				cur_o		= lev.get_object(x, y, i);
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

		image_buffer_ptr->apply(gra.object_layer_buffer, object_level_pos.x, object_level_pos.y);
	}
}

/*
 * Editor step handler
 */
void editor_event_handler::e_step(int delta_t) {
	vec		pos			= _floating_window_pos;
	uint	scr_width	= gra.SCREEN_WIDTH;
	uint	scr_height	= gra.SCREEN_HEIGHT;
	uint	lev_width	= lev.get_pixel_width();
	uint	lev_height	= lev.get_pixel_height();
	uint	x			= _mouse_x;
	uint	y			= _mouse_y;

	// Insert object at position when left mouse is down and when in insertion
	if(_left_mouse_is_down && _state == STATE_INSERTION) {
		coords level_pos = gam.level_pos_from_window_pos(_mouse_x, _mouse_y);
		lev.insert_obj_at_pixel(_sel_obj_type, level_pos.x, level_pos.y, _can_edit_const);
	}

	if(_right_mouse_is_down && _state == STATE_DEFAULT) {
		coords del_pos = gam.level_pos_from_window_pos(_mouse_x, _mouse_y);
		lev.remove_obj_at_pixel(del_pos.x, del_pos.y, _can_edit_const);
	}

	// Scroll left?
	if(x >= 0 && x < SCROLL_AREA_SIZE)
		pos.x -= _scroll_distance(x, delta_t);
	// Scroll right?
	if(x < scr_width && x >= scr_width - SCROLL_AREA_SIZE)
		pos.x += _scroll_distance(scr_width - x, delta_t);
	// Scroll up?
	if(y >= 0 && y < SCROLL_AREA_SIZE)
		pos.y -= _scroll_distance(y, delta_t);
	// Scroll down?
	if(y < scr_height && y >= scr_height - SCROLL_AREA_SIZE)
		pos.y += _scroll_distance(scr_height - y, delta_t);

	// Keep window in bounds
	if(pos.x < 0)
		pos.x = 0;
	if(pos.x + scr_width > lev_width)
		pos.x =  lev_width - scr_width;
	if(pos.y < 0)
		pos.y = 0;
	if(pos.y + scr_height > lev_height)
		pos.y =  lev_height - scr_height;
	_floating_window_pos = pos;
	
	if(floor(pos.x) != gam.get_window_pos().x || floor(pos.y) != gam.get_window_pos().y ) {
		gam.set_window_pos((uint)floor(pos.x), (uint)floor(pos.y));
		_scrolled = true;
	}
}
double editor_event_handler::_scroll_distance(int mouse_offset, uint delta_t) {
	return pow(2.718, -0.1*(mouse_offset - 1)) * MAX_SCROLL_SPEED * 0.001 * delta_t;
}

/*
 * Get mouse coordinates
 */
coords	editor_event_handler::_mouse_coords() {
	return coords(_mouse_x, _mouse_y);
}

/*
 * Start simulation
 */
bool editor_event_handler::start_simulation() {
	if(!lev.cannon_exists()) 
		return false;

	cur_eh = &sim_eh;
	sim_eh.init(_can_edit_const);
	return true;
}

/*
 * Set level/game editor
 */
void editor_event_handler::set_mode(bool constant) {
	_can_edit_const = constant;
}

/*
 * Get level/game editor
 */
bool editor_event_handler::get_mode(){
	return _can_edit_const;
}