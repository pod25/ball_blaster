/*
 * File: graphics.cpp
 */
#include "common.h"

/*
 * Initialize image buffers
 */
void graphics::init_images(bool load_images) {
	std_background_img		= "background.png";
	std_menu_background_img	= "menu_background.png";
	std_ball_img			= "ball_base.png";
	std_ball_lighting_img	= "ball_lighting.png";
	std_obj_images[OC_WALL	][DIR_NODIR	] = "wall.png";
	std_obj_images[OC_GOAL	][DIR_NODIR	] = "goal.png";
	std_obj_images[OC_CANNON][DIR_NODIR	] = "ball.png";
	std_obj_images[OC_FAN	][DIR_LEFT	] = "fan_left.png";
	std_obj_images[OC_FAN	][DIR_UP	] = "fan_up.png";
	std_obj_images[OC_FAN	][DIR_RIGHT	] = "fan_right.png";
	std_obj_images[OC_FAN	][DIR_DOWN	] = "fan_down.png";
	std_obj_images[OC_MAGNET][DIR_LEFT	] = "magnet_left.png";
	std_obj_images[OC_MAGNET][DIR_UP	] = "magnet_up.png";
	std_obj_images[OC_MAGNET][DIR_RIGHT	] = "magnet_right.png";
	std_obj_images[OC_MAGNET][DIR_DOWN	] = "magnet_down.png";

	if (load_images) {
		screen_buffer			.init(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SCREEN_FLAGS);
		background_buffer		.load(std_background_img);
		menu_background_buffer	.load(std_menu_background_img);
		init_object_images		(lev.LEVEL_DEFAULT_GRID_SIZE / 2);
	}
}

/*
 * Initialize object buffers
 */
void graphics::init_object_images(int grid_size) {
	if (!grid_size) grid_size = lev.LEVEL_DEFAULT_GRID_SIZE;
	for(uint i = 0; i < NUM_OBJECT_CLASSES; i++)
		for(uint d = 0; d < NUM_DIRECTIONS; d++)
			if(!std_obj_images[i][d].empty()) {
				object_buffers[i][d].load(std_obj_images[i][d]);
				object_buffers[i][d].resize(grid_size, grid_size);
			}
}

/*
 * Initialize ball buffer
 */
void graphics::init_ball_image(double new_diameter) {
	ball_buffer.load(std_ball_img);
	ball_lighting_buffer.load(std_ball_lighting_img);
	ball_lighting_buffer.resize(new_diameter, new_diameter);
}

/*
 * Get the size of the ball image
 */
uint graphics::get_ball_img_size() {
	return ball_buffer.get_sdl_srf()->w;
}

/*
 * Output to screen
 */
void graphics::update() {
	screen_buffer.flip();
}

/*
 * Get screen buffer
 */
video_mode& graphics::get_screen_buffer() {
	return screen_buffer;
}

/*
 * Constructor
 */
graphics::graphics() {
	try {
		init_images(true);

		menu_font.load("Fonts/lazy.ttf", 28);
		menu_color.r = 0; menu_color.g = 0; menu_color.b = 0;
		menu_color_selected.r = 255; menu_color_selected.g = 255; menu_color_selected.b = 255;
	}
	catch (exception &e) {
		err_hndl_obj.inform_about_error("graphics default constructor", e);
	};
}

/*
 * Resize object layer
 */
void graphics::set_object_layer_size(size_t w, size_t h) {
	if(!object_layer_buffer.empty())
		object_layer_buffer.free();
	object_layer_buffer	.generate_rect(w, h);
	object_layer_buffer	.enable_alpha();
	editor_eh			.objects_changed(0, 0, true);
}

/*
 * Redraw object buffers when grid size is changed
 */
void graphics::set_grid_size(uint grid_size) {
	init_object_images(grid_size);
}