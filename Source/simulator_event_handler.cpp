/*
 * File: simulator_event_handler.cpp
 */
#include "common.h"

/*
 * Simulator constructor
 */
simulator_event_handler::simulator_event_handler() {
}

/*
 * Initialize simulation
 */
void simulator_event_handler::init(bool from_editor) {
	phy.init_level_simulation();
	_from_editor = from_editor;
	refresh_obj_layer();
	gra.init_ball_image(lev.get_grid_size() * 1.0 / lev.LEVEL_DEFAULT_GRID_SIZE * lev.get_ball_scale());
	_follow_ball();
	_state = STATE_RUNNING;
}

/*
 * Follow the ball
 */
void simulator_event_handler::_follow_ball() {
	coords	ball;
	ball.x = (int)lev.get_ball_pos().x;
	ball.y = (int)lev.get_ball_pos().y;
	coords	pos			= _window_pos;
	int		scr_width	= gra.SCREEN_WIDTH;
	int		scr_height	= gra.SCREEN_HEIGHT;
	int		lev_width	= lev.get_pixel_width();
	int		lev_height	= lev.get_pixel_height();

	// Ball getting close to any edge?
	if(ball.x - pos.x < FOLLOW_LIMIT)
		pos.x = ball.x - FOLLOW_LIMIT;
	if(ball.x > pos.x + scr_width - FOLLOW_LIMIT)
		pos.x = ball.x - scr_width + FOLLOW_LIMIT;
	if(ball.y - pos.x < FOLLOW_LIMIT)
		pos.y = ball.y - FOLLOW_LIMIT;
	if(ball.y > pos.y + scr_height - FOLLOW_LIMIT)
		pos.y = ball.y - scr_height + FOLLOW_LIMIT;

	// Keep window in bounds
	if(pos.x < 0)
		pos.x = 0;
	if(pos.x + scr_width > lev_width)
		pos.x =  lev_width - scr_width;
	if(pos.y < 0)
		pos.y = 0;
	if(pos.y + scr_height > lev_height)
		pos.y =  lev_height - scr_height;
	_window_pos = pos;
}

/*
 * Simulator mouse movement handler
 */
void simulator_event_handler::e_mouse_move(int mouse_x, int mouse_y) {
}

/*
 * Simulator mouse button handlers
 */
void simulator_event_handler::e_mouse_down(int mouse_x, int mouse_y, int button) {
}
void simulator_event_handler::e_mouse_up(int mouse_x, int mouse_y, int button) {
}

/*
 * Simulator key button handlers
 */
void simulator_event_handler::e_key_down(int key) {
	switch(_state) {
		case STATE_RUNNING:
			if(key == SDLK_ESCAPE) {
				cur_eh = &editor_eh;
				editor_eh.set_mode(_from_editor);
				editor_eh.objects_changed(0, 0, true);
			}
			break;
		case STATE_COMPLETE:
			cur_eh = &menu_eh;
			menu_eh.menu_reset();
			break;
	}
}
void simulator_event_handler::e_key_up(int key) {
}

/*
 * Simulator new frame handler
 */
void simulator_event_handler::e_new_frame() {
	// Refresh screen while running
	if(_state == STATE_RUNNING) {
		gra.background_buffer.apply(0, 0);

		// Apply object layer to screen
		SDL_Rect src_rect;
		src_rect.x = gam.get_window_pos().x;
		src_rect.y = gam.get_window_pos().y;
		src_rect.w = gra.SCREEN_WIDTH;
		src_rect.h = gra.SCREEN_HEIGHT;
		gra.object_layer_buffer.apply(0, 0, &src_rect);

		// Apply ball buffer
		int ball_size = lev.get_ball_pixel_size();
		coords	ball;
		ball = vec_to_coords(negated_y(lev.get_ball_pos()*lev.get_pixels_per_le()) - ball_size/2*vec(1, 1));
		gra.ball_buffer.apply(ball.x, ball.y);

		// Refresh screen
		gra.update();
	}
}

/*
 * Refresh object layer
 */
void simulator_event_handler::refresh_obj_layer() {
	// Clear whole buffer
	gra.object_layer_buffer.clear();

	// Refresh object layer
	for(size_t cur_x = 0; cur_x < lev.get_width(); cur_x++) {
		for(size_t cur_y = 0; cur_y < lev.get_height(); cur_y++) {
			_plot_square(cur_x, cur_y);
		}
	}
}

/*
 * Refresh square in object layer
 */
void simulator_event_handler::_plot_square(size_t x, size_t y) {
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

		if(cannon_o)
			return;
		else if(wall_o)
			image_buffer_array = gra.object_buffers[OC_WALL];
		else if(goal_o)
			image_buffer_array = gra.object_buffers[OC_GOAL];
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
 * Simulator step handler
 */
void simulator_event_handler::e_step(int delta_t) {
	if(_state == STATE_RUNNING)
		phy.step(delta_t / 1000.0);
}

/*
 * Level complete
 */
void simulator_event_handler::level_complete() {
	_state = STATE_COMPLETE;
	image completed;
	completed.generate_text("Congratulations! You have completed the level!", gra.menu_font, gra.menu_color);
	completed.apply(200, 300);
	completed.generate_text("Press any key to continue", gra.menu_font, gra.menu_color_selected);
	completed.apply(310, 340);
	gra.update();
}