/*
 * File: graphics.cpp
 */
#include "common.h"

void graphics::init_images(bool load_images) {
	std_background_img	= "background.png";
	std_ball_img		= "Ball/Black ball.png";
	std_obj_images[OC_WALL	][DIR_NODIR	] = "wall.png";
	std_obj_images[OC_GOAL	][DIR_NODIR	] = "goal.png";
	std_obj_images[OC_CANNON][DIR_NODIR	] = "cannon.png";
	std_obj_images[OC_FAN	][DIR_LEFT	] = "fan_left.png";
	std_obj_images[OC_FAN	][DIR_UP	] = "fan_up.png";
	std_obj_images[OC_FAN	][DIR_RIGHT	] = "fan_right.png";
	std_obj_images[OC_FAN	][DIR_DOWN	] = "fan_down.png";
	std_obj_images[OC_MAGNET][DIR_LEFT	] = "magnet_left.png";
	std_obj_images[OC_MAGNET][DIR_UP	] = "magnet_up.png";
	std_obj_images[OC_MAGNET][DIR_RIGHT	] = "magnet_right.png";
	std_obj_images[OC_MAGNET][DIR_DOWN	] = "magnet_down.png";
	if (load_images) {
		screen_buffer		.init(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SCREEN_FLAGS);
		background_buffer	.load(std_background_img);
		object_layer_buffer	.generate_rect(SCREEN_WIDTH, SCREEN_HEIGHT);
		ball_buffer			.load(std_ball_img);
		for(uint i = 0; i < NUM_OBJECT_CLASSES; i++)
			for(uint d = 0; d < NUM_DIRECTIONS; d++)
				if(!std_obj_images[i][d].empty())
					object_buffers[i][d].load(std_obj_images[i][d]);
	}
}

void graphics::update() {
	screen_buffer.flip();
}

video_mode& graphics::get_screen_buffer() {return screen_buffer;}

graphics::graphics() {
	try {
		init_images(true);
		refresh = false;
	}
	catch (exception &e) {err_hndl_obj.inform_about_error("graphics default constructor", e);};
}
