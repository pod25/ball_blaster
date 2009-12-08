/*
 * File: simulator_event_handler.cpp
 */
#include "common.h"

/*
 * Simulator constructor
 */
simulator_event_handler::simulator_event_handler() {

}
void simulator_event_handler::init(bool from_editor) {
	phy.init_level_simulation();
	_from_editor = from_editor;
	refresh_obj_layer();
	gra.init_ball_image(lev.get_grid_size() * 1.0 / lev.LEVEL_DEFAULT_GRID_SIZE * lev.get_ball_scale());
	_follow_ball();
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
	if(key == SDLK_ESCAPE) {
		cur_eh = &editor_eh;
		editor_eh.set_mode(_from_editor);
	}
}
void simulator_event_handler::e_key_up(int key) {

}

/*
 * Simulator new frame handler
 */
void simulator_event_handler::e_new_frame() {
	gra.background_buffer.apply(0, 0);

	SDL_Rect src_rect;
	src_rect.x = gam.get_window_pos().x;
	src_rect.y = gam.get_window_pos().y;
	src_rect.w = gra.SCREEN_WIDTH;
	src_rect.h = gra.SCREEN_HEIGHT;

	gra.object_layer_buffer.apply(0, 0, &src_rect);
	int ball_size = lev.get_ball_pixel_size();
	coords	ball;
	ball.x = (int)lev.get_ball_pos().x - ball_size / 2;
	ball.y = (int)lev.get_ball_pos().y - ball_size / 2;
	gra.ball_buffer.apply(ball.x, ball.y);

	gra.update();
}

/*
 * Refresh object layer
 */
void simulator_event_handler::refresh_obj_layer() {
	// Clear whole buffer
	gra.object_layer_buffer.clear();

	for(size_t cur_x = 0; cur_x < lev.get_width(); cur_x++) {
		for(size_t cur_y = 0; cur_y < lev.get_height(); cur_y++) {
			_plot_square(cur_x, cur_y);
		}
	}
}
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
 * Simulator step handler
 */
void simulator_event_handler::e_step(int delta_t) {
	phy.step(delta_t / 1000.0);
}