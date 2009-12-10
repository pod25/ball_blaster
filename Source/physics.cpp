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

void physics::reflect_ball_vel(double dt) {
	vec		vel			= lev.get_ball_vel();
	vec		bnorm		= next_bounce._h_normal;
	lev.set_ball_vel(vel - (vel*bnorm)*bnorm/bnorm.sqr_length()*(1 + bounce_coefficient));
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

void physics::hit_test_line(int hit_type, vec lp1, vec ldp, vec bp1, vec bdp) {

}

void physics::hit_test_circle(int hit_type, vec cp, double crad, vec bp1, vec bdp) {
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
	if (hit_type == HIT_BOUNCE && tmid < 0) return; //Ball is already half way through the circle;
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

void physics::step_dividing(double dt, bool iterate_each_bounce) {
	double t_left = dt;
	double it_dt; // Delta t for this iteration
	vec bv;
	vec bp1;
	vec bp2;
	if (!iterate_each_bounce) apply_ball_acceleration(dt, .5);
	for (uint num_moves_left = iterate_each_bounce ? MAX_MOVES_PER_STEP : MAX_MOVES_PER_STEP_ON_FAILURE; num_moves_left && t_left != 0; num_moves_left--) {
		vec obpos = lev.get_ball_pos(); // Backup of ball position
		vec obvel = lev.get_ball_vel(); // Backup of ball velocity
		it_dt = t_left;
		for (int curr_b_it = 0; curr_b_it < NUM_BOUNCES_ITERATIONS && iterate_each_bounce || !curr_b_it; curr_b_it++) {
			lev.set_ball_pos(obpos);
			lev.set_ball_vel(obvel);
			calculate_ball_acceleration(); // Calculate ball acceleration for this bounce
			if (iterate_each_bounce) apply_ball_acceleration(it_dt, .5);
			bounce_detected		= false;
			in_goal_this_step	= false;
			// Extract position and speed and calculate new position
			bv  = lev.get_ball_vel();
			bp1 = lev.get_ball_pos();
			bp2 = bp1 + t_left*bv;
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
			if (bounce_detected) it_dt = next_bounce._t * t_left;
			else                 break;
		} // curr_bounce_iteration
		lev.set_ball_pos(bp1 + it_dt*bv);
		calculate_ball_acceleration();
		if (iterate_each_bounce) apply_ball_acceleration(it_dt, .5);
		if (bounce_detected) reflect_ball_vel(t_left); // Moves ball
		t_left -= it_dt;
	} // num_calls_left
	if (t_left != 0) { // Had to break because ball bounced to much (it's possibly rolling)
#if 0 // Anti-freeze measure method 1 ()
		time_taken -= t_left;
		lev.set_ball_vel(bv + .1*rot_90_deg_ccw(bv)); // Rotate velocity a litte bit to make the ball get out of the dead lock
#else // Anti-freeze measure method 1 (calculate the rest of the step without iterating each bounces)
		if (iterate_each_bounce) { // This will cause the ball to freeze when rolling since it's bouncing infinitely much
			for (int i = 0; i < NUM_SUB_STEPS_ON_FAILURE; i++) { // Divide the time that's left on NUM_SUB_STEPS_ON_FAILURE new steps, without bounce iteration
				step_dividing(t_left/NUM_SUB_STEPS_ON_FAILURE, false); // Shouldn't make the ball freeze because of rolling
			}
		}
		else {
			lev.set_ball_vel(vec(0,0)); // Stop ball, maybe it has got stuck
			time_taken -= t_left; // This would be unexpected
		}
#endif
	}
	if (!iterate_each_bounce) apply_ball_acceleration(dt, .5);
}

void physics::init_level_simulation() {
	time_taken		= 0;
	goal_reached	= false;
	ball_rad		= lev.get_square_scale()*lev.get_ball_scale()/2;
	lev.set_ball_pos(negated_y(vec(lev.cannon_coords()) + vec(0.5, 0.5))*lev.get_square_scale());
	lev.set_ball_vel(vec(lev.get_cannon()->_shot_vec) * CANNON_STRENGH);
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
	step_dividing(dt);
	// Check for goal hit
	if (in_goal_this_step) goal_reached = true;
}