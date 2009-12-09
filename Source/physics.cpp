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

void physics::bounce_ball(double& dt) {
	double	t_to_bounce	= dt * next_bounce._t;
	vec		vel			= lev.get_ball_vel();
	vec		bnorm		= next_bounce._h_normal;
	lev.set_ball_pos(lev.get_ball_pos() + t_to_bounce * vel);
	lev.set_ball_vel(vel - (vel*bnorm)*bnorm/bnorm.sqr_length()*(1 + bounce_coefficient));
	dt *= (1 - t_to_bounce);
}

void physics::report_hit_event(int hit_type, hit_event he) {
	if (bounce_detected && next_bounce._t <= he._t) return; // Bounce earlier or equally early detected
	switch (hit_type) {
		case HIT_BOUNCE:
			bounce_detected = true;
			next_bounce = he;
			if (he._t < next_goal_hit._t) in_goal_this_step = false;
			break;
		case HIT_GOAL:
			if (next_bounce._t < he._t) break;
			in_goal_this_step = true;
			next_goal_hit = he;
			break;
	}
}

void physics::hit_test_line(int hit_type, vec lp1, vec dlp, vec bp1, vec bdp) {

}

void physics::hit_test_circle(int hit_type, vec cp, double crad, vec bp1, vec bdp, bool will_bounce) {
	double	bdp_sq				= bdp.sqr_length();
	if (!bdp_sq) return; // The ball isn't moving
	vec		bp1_to_c			= cp - bp1; // Form ball mid to circle mid
	double	tot_rad				= crad + ball_rad; // Total radius
	double	tot_rad_sq			= tot_rad*tot_rad;
	double	b_line_to_c_sq		= square(bp1_to_c % bdp)/bdp_sq;
	if (b_line_to_c_sq >= tot_rad_sq) return; // Circle is to far from movement
	// Solve equation for when the ball hits the circle
	double	half_tang_spots_diff_sq; // Square of half the distance between the points where the ball and the circle will tangent each other
	half_tang_spots_diff_sq		= tot_rad_sq - b_line_to_c_sq;
	double tmid					= (bp1_to_c)*bdp/bdp_sq; // Middle of the times for the two tangent events
	if (will_bounce && tmid < 0) return; //Ball is already half way through the circle;
	double t1					= tmid - sqrt(half_tang_spots_diff_sq/bdp_sq);
	double t2					= tmid + sqrt(half_tang_spots_diff_sq/bdp_sq);
	if (t1 > 1 || t2 < 0) return; //Ball will not interfere with circle during this frame
	report_hit_event(hit_type, hit_event(t1, bp1 + t1*bdp - cp));
}

void physics::hit_test_obj(int hit_type, vec ul_crnr_pos, vec bp1, vec bdp) {
	// Bounce check sides
	hit_test_line(hit_type, ul_crnr_pos                                   , lev.get_square_scale()*vec( 0,-1), bp1, bdp);
	hit_test_line(hit_type, ul_crnr_pos + lev.get_square_scale()*vec(0,-1), lev.get_square_scale()*vec( 1, 0), bp1, bdp);
	hit_test_line(hit_type, ul_crnr_pos + lev.get_square_scale()*vec(1,-1), lev.get_square_scale()*vec( 0, 1), bp1, bdp);
	hit_test_line(hit_type, ul_crnr_pos + lev.get_square_scale()*vec(1, 0), lev.get_square_scale()*vec(-1, 0), bp1, bdp);
	// Bounce check corners
	hit_test_circle(hit_type, ul_crnr_pos                                   , 0, bp1, bdp);
	hit_test_circle(hit_type, ul_crnr_pos + lev.get_square_scale()*vec(0,-1), 0, bp1, bdp);
	hit_test_circle(hit_type, ul_crnr_pos + lev.get_square_scale()*vec(1,-1), 0, bp1, bdp);
	hit_test_circle(hit_type, ul_crnr_pos + lev.get_square_scale()*vec(1, 0), 0, bp1, bdp);
}

void physics::step(double dt, uint num_calls_left) {
	for (; num_calls_left; num_calls_left--) {
		bounce_detected		= false;
		in_goal_this_step	= false;
		// Extract position and speed and calculate new position
		vec bv  = lev.get_ball_vel();
		vec bp1 = lev.get_ball_pos();
		vec bp2 = bp1 + dt*bv;
		// Calculate traveling rect
		bp1 = negated_y(bp1);
		bp2 = negated_y(bp2);
		size_t x1 = size_t((min(bp1.x, bp2.x) - ball_rad) / lev.get_square_scale())    ;
		size_t x2 = size_t((max(bp1.x, bp2.x) + ball_rad) / lev.get_square_scale()) + 1;
		size_t y1 = size_t((min(bp1.y, bp2.y) - ball_rad) / lev.get_square_scale())    ;
		size_t y2 = size_t((max(bp1.y, bp2.y) + ball_rad) / lev.get_square_scale()) + 1;
		bp1 = negated_y(bp1);
		bp2 = negated_y(bp2);
		size_t x, y;
		object* curr_obj;
		for (y = y1; y < y2; y++) {
			for (x = x1; x < x2; x++) {
				if (!lev.num_objects(x, y)) continue;
				curr_obj = lev.get_object(x, y, 0);
				if      (dynamic_cast<wall*>(curr_obj)) hit_test_obj(HIT_BOUNCE, vec(x, -int(y))*lev.get_square_scale(), bp1, bp2-bp1);
				else if (dynamic_cast<goal*>(curr_obj)) hit_test_obj(HIT_GOAL  , vec(x, -int(y))*lev.get_square_scale(), bp1, bp2-bp1);
			}
		}
		if (!bounce_detected) {
			lev.set_ball_pos(bp2);
			return;
		}
		bounce_ball(dt);
	}
	lev.set_ball_vel(vec(0, 0)); // Stop ball, it have probably got stuck
}

void physics::init_level_simulation() {
	time_taken		= 0;
	goal_reached	= false;
	ball_rad		= lev.get_square_scale()*lev.get_ball_scale()/2;
	lev.set_ball_pos(negated_y(vec(lev.cannon_coords()) + vec(0.5, 0.5))*lev.get_square_scale());
	lev.set_ball_vel(vec(lev.get_cannon()->_shot_vec) * CANNON_STRENGH);
	calculate_ball_acceleration();
}

void physics::step(double dt) {
	if (goal_reached) {
		dt = -min(dt, time_taken);
		bounce_coefficient = BOUNCE_COEFFICIENT ? 1/BOUNCE_COEFFICIENT : BOUNCE_COEFFICIENT;
	}
	else {
		bounce_coefficient = BOUNCE_COEFFICIENT;
	}
	time_taken += dt;
	// Update position, velocity and acceleration of the ball
	apply_ball_acceleration(dt, .5);
	step(dt, MAX_FRAME_ITERATIONS);
	apply_ball_acceleration(dt, .5);
	// Check for goal hit
	if (in_goal_this_step) goal_reached = true;
}