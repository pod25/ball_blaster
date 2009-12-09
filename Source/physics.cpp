/*
 * File: physics.cpp
 */
#include "common.h"

void physics::calculate_ball_acceleration() {
	ball_acc = lev.get_gravity();
}

void physics::apply_ball_acceleration(double dt, double amount) {
	lev.set_ball_vel(lev.get_ball_vel() + amount * dt * ball_acc);
}

void physics::bounce_check_obj(object* curr_obj, vec p1, vec v) {
	//if ()
}

void physics::bounce_ball() {
}

void physics::step(double dt, uint num_calls_left) {
	for (; num_calls_left; num_calls_left--) {
		bounce_detected = false;
		//Calculate new position
		vec v  = lev.get_ball_vel();
		vec p1 = lev.get_ball_pos();
		vec p2 = p1 + dt*v;
		// Calculate traveling rect
		size_t x1 = size_t((min(p1.x, p2.x) - ball_rad) / lev.get_square_scale())    ;
		size_t x2 = size_t((max(p1.x, p2.x) + ball_rad) / lev.get_square_scale()) + 1;
		size_t y1 = size_t((min(p1.y, p2.y) - ball_rad) / lev.get_square_scale())    ;
		size_t y2 = size_t((max(p1.y, p2.y) + ball_rad) / lev.get_square_scale()) + 1;
		size_t x, y;
		object* curr_obj;
		for (y = y1; y < y2; y++) {
			for (x = x1; x < x2; x++) {
				if (!lev.num_objects(x, y)) continue;
				curr_obj = lev.get_object(x, y, 0);
				if (dynamic_cast<wall*>(curr_obj) ||
					dynamic_cast<goal*>(curr_obj) ) bounce_check_obj(curr_obj, p1, v);
			}
		}
		if (!bounce_detected) {
			lev.set_ball_pos(p2);
			return;
		}
		bounce_ball();
	}
	lev.set_ball_vel(vec(0, 0)); // Stop ball, it have probably got stuck
}

void physics::init_level_simulation() {
	ball_rad = lev.get_ball_scale()/2;
	lev.set_ball_pos((vec(lev.cannon_coords()) + vec(0.5, 0.5))*lev.get_square_scale());
	lev.set_ball_vel(vec(lev.get_cannon()->_shot_vec) * CANNON_STRENGH);
	calculate_ball_acceleration();
}

void physics::step(double dt) {
	apply_ball_acceleration(dt, .5);
	step(dt, MAX_FRAME_ITERATIONS);
	apply_ball_acceleration(dt, .5);
}