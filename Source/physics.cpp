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

bool physics::bounce_check_line(vec lp1, vec dlp, vec bp1, vec bv) {
	return false;
}

bool physics::bounce_check_obj(vec ul_crnr_pos, vec bp1, vec bv) {
	bool b = false; // Bounced
	b |= bounce_check_line(ul_crnr_pos                                   , lev.get_square_scale()*vec( 0,-1), bp1, bv);
	b |= bounce_check_line(ul_crnr_pos + lev.get_square_scale()*vec(0,-1), lev.get_square_scale()*vec( 1, 0), bp1, bv);
	b |= bounce_check_line(ul_crnr_pos + lev.get_square_scale()*vec(1,-1), lev.get_square_scale()*vec( 0, 1), bp1, bv);
	b |= bounce_check_line(ul_crnr_pos + lev.get_square_scale()*vec(1, 0), lev.get_square_scale()*vec(-1, 0), bp1, bv);
	return b;
}

void physics::bounce_ball() {
}

void physics::step(double dt, uint num_calls_left) {
	for (; num_calls_left; num_calls_left--) {
		bounce_detected = false;
		//Calculate new position
		vec bv  = lev.get_ball_vel();
		vec bp1 = lev.get_ball_pos();
		vec bp2 = bp1 + dt*bv;
		// Calculate traveling rect
		size_t x1 = size_t((min(bp1.x, bp2.x) - ball_rad) / lev.get_square_scale())    ;
		size_t x2 = size_t((max(bp1.x, bp2.x) + ball_rad) / lev.get_square_scale()) + 1;
		size_t y1 = size_t((min(bp1.y, bp2.y) - ball_rad) / lev.get_square_scale())    ;
		size_t y2 = size_t((max(bp1.y, bp2.y) + ball_rad) / lev.get_square_scale()) + 1;
		size_t x, y;
		object* curr_obj;
		for (y = y1; y < y2; y++) {
			for (x = x1; x < x2; x++) {
				if (!lev.num_objects(x, y)) continue;
				curr_obj = lev.get_object(x, y, 0);
				if (dynamic_cast<wall*>(curr_obj) || dynamic_cast<goal*>(curr_obj) ) {
					if (bounce_check_obj(vec(x, -y)*lev.get_square_scale(), bp1, bv) &&
						dynamic_cast<goal*>(curr_obj) ) {
							goal_reached = true;
					}
				}
			}
		}
		if (!bounce_detected) {
			lev.set_ball_pos(bp2);
			return;
		}
		bounce_ball();
	}
	lev.set_ball_vel(vec(0, 0)); // Stop ball, it have probably got stuck
}

void physics::init_level_simulation() {
	goal_reached = false;
	ball_rad = lev.get_ball_scale()/2;
	lev.set_ball_pos(negated_y(vec(lev.cannon_coords()) + vec(0.5, 0.5))*lev.get_square_scale());
	lev.set_ball_vel(vec(lev.get_cannon()->_shot_vec) * CANNON_STRENGH);
	calculate_ball_acceleration();
}

void physics::step(double dt) {
	//return;
	apply_ball_acceleration(dt, .5);
	step(dt, MAX_FRAME_ITERATIONS);
	apply_ball_acceleration(dt, .5);
}