/*
 * File: graphics.cpp
 */
#include "common.h"

void graphics::init_images(bool load_images) {
	std_background_img	= "Background/tb_jungle_guatemala.jpg";
	std_ball_img		= "Ball/Black ball.png";
	std_obj_images[ OC_WALL			] = "Wall/Wall SMB2.png";
	std_obj_images[ OC_GOAL_AREA	] = "Wall/Mushrom SMWar.png";
	std_obj_images[ OC_CANNON		] = "Cannon/Artillery_gun_crew-illustration.jpeg";
	std_obj_images[ OC_FAN			] = "Fan/tokyo_plusminuszero_fan2.png";
	std_obj_images[ OC_MAGNET		] = "Magnet/horshoemagnet1.jpg";
	if (load_images) {
		screen_buffer		.init(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SCREEN_FLAGS);
		if(screen_buffer.empty()) throw exception("Couldn't set up screen");
		background_buffer	.load(std_background_img);
		ball_buffer			.load(std_ball_img);
		for (uint i = 0; i < NUM_OBJECT_CLASSES; i++) object_buffers[i].load(std_obj_images[i]);
	}
}

void graphics::set_refresh_flag() {
	refresh = true;
}

void graphics::update() {
	if (!refresh) return;
	screen_buffer.flip();
	refresh = false;
}

video_mode& graphics::get_screen_buffer() {return screen_buffer;}

graphics::graphics() {
	try {
		init_images(true);
		refresh = false;
	}
	catch (exception &e) {err_hndl_obj.inform_about_error("graphics default constructor", e);};
}
