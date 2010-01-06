/*
 * File: physics.cpp
 */
#include "common.h"

void physics::calculate_ball_acceleration() {
	ball_acc = lev.get_gravity();

	// Calculate acceleration created by fan or magnet
	vec		d_ball_square = negated_y(lev.get_ball_pos())/lev.get_square_scale() - vec(1,1)/2;
	coords	  ball_square = vec_to_coords(d_ball_square);
	int c;
	int i;
	// Check row
	int r = ball_square.y;
	for(c = 0; c < (int)lev.get_width(); c++) {
		for(i = 0; i < (int)lev.num_objects(c, r); i++) {
			// Directed object?
			directed_object* d_o = dynamic_cast<directed_object*>(lev.get_object(c, r, i));
			if(d_o) {
				int dir = d_o->get_dir();
				if((dir == DIR_LEFT && ball_square.x <= c) || (dir == DIR_RIGHT && ball_square.x >= c)) {
					int step = (dir == DIR_LEFT) ? -1 : 1;
					bool found_wall = false;
					int d;
					for(d = 0; c + d != ball_square.x ; d += step)
						if(lev.is_wall(c + d, r))
							found_wall = true;
					if(!found_wall) {
						magnet*	m = dynamic_cast<magnet*>(d_o);
						fan*	f = dynamic_cast<fan*>(d_o);
						if(m)
							ball_acc.x -= step * m->_strength / (1 + abs(d));
						else if(f)
							ball_acc.x += step * f->_strength / (1 + abs(d));
					}
				}
			}
		}
	}
	// Check column
	c = ball_square.x;
	for(r = 0; r < (int)lev.get_height(); r++) {
		for(i = 0; i < (int)lev.num_objects(c, r); i++) {
			// Directed object?
			directed_object* d_o = dynamic_cast<directed_object*>(lev.get_object(c, r, i));
			if(d_o) {
				int dir = d_o->get_dir();
				if((dir == DIR_UP && ball_square.y <= r) || (dir == DIR_DOWN && ball_square.y >= r)) {
					int step = (dir == DIR_DOWN) ? 1 : -1;
					bool found_wall = false;
					int d;
					for(d = 0; r + d != ball_square.y ; d += step)
						if(lev.is_wall(c, r + d))
							found_wall = true;
					if(!found_wall) {
						magnet*	m = dynamic_cast<magnet*>(d_o);
						fan*	f = dynamic_cast<fan*>(d_o);
						if(m)
							ball_acc.y += step * m->_strength / (1 + abs(d));
						else if(f)
							ball_acc.y -= step * f->_strength / (1 + abs(d));
					}
				}
			}
		}
	}
}

void physics::apply_ball_acceleration(double dt, double amount) {
	lev.set_ball_vel(lev.get_ball_vel() + amount * dt * ball_acc);
}

void physics::move_and_rotate_ball(double dt) {
	lev.set_ball_pos(lev.get_ball_pos() + dt*lev.get_ball_vel());
	lev.set_ball_ang(lev.get_ball_ang() + dt*lev.get_ball_ang_vel());
}

void physics::bounce_ball_vel_and_ang_vel(double dt) { // Assume ball has mass = 1
	vec		vel			= lev.get_ball_vel(); // [l.u./sek]
	vec		bnnorm		= next_bounce._h_normal.normalized(); // [1]
	double	speed_ort	= -(vel * bnnorm); // [l.u./sek] Dot product will be negative
	vec		vel_ort		= -speed_ort * bnnorm; // [l.u./sek]
	vec		vel_par		= vel - vel_ort; // [l.u./sek]
	double	speed_par	= bnnorm % vel_par; // [l.u./sek] In counter-clockwise direction "around" the surface
	double	bc			= speed_factor < 0 && bounce_coefficient   ? 1/bounce_coefficient   : bounce_coefficient  ; // [1]
	double	mnm			= speed_factor < 0 && max_negated_momentum ? 1/max_negated_momentum : max_negated_momentum; // [1]
	double	ort_dp		= speed_ort * (1 + bc) * BALL_MASS; // [kg*l.u./sek]
	double	max_par_dp	= friction_coefficient * ort_dp; // [kg*l.u./sek]
	double	ang_vel		= lev.get_ball_ang_vel(); // [1/sek]
	double	rel_ang_vel	= ang_vel - speed_par/ball_rad; // [1/sek]
	double	rel_ang_mom	= rel_ang_vel * ball_moment_of_inertia; // [kg*l.u.^2/sek]
	double	par_dp		= min(max_par_dp, abs(rel_ang_mom/ball_rad) * (1 + mnm)) * sign(rel_ang_mom); // [kg*l.u./sek]
	double	new_ang_vel	= ang_vel - par_dp * ball_rad/ball_moment_of_inertia; // [1/sek]
	vec		new_vel_par	= vel_par + par_dp * rotated_90_deg_ccw(bnnorm); // [l.u./sek]
	vec		new_vel_ort = -bc * vel_ort;

	lev.set_ball_vel(new_vel_par + new_vel_ort);
	lev.set_ball_ang_vel(new_ang_vel);
}

void physics::report_hit_event(int hit_type, hit_event he) {
	if (he._t > 1) return; // Bounce doesn't occur this step
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
	vec		bp1_to_lp1		= lp1 - bp1;
	double	bp1_along_line	= bp1_to_lp1 * ldp;
	double	bdp_along_line	= bdp * ldp;
	double	line_sq_len		= ldp.sqr_length();
	if (bp1_along_line > 0 && bdp_along_line < 0 || bp1_along_line < -line_sq_len && bdp_along_line > 0)
		return; // The ball is outside the edges of the line and is moving from them (prevents negative t bounces)
	if ((bp1_to_lp1 % ldp) * (bdp % ldp) <= 0) return; // The center of the ball isn't moving towards the line
	vec		nnorm			= rotated_90_deg_cw(ldp).normalized(); // Normalized normal of the bounce surface
	double	hit_t			= (bp1_to_lp1 * nnorm + ball_rad) / (bdp * nnorm); // The dot products should be negative
	double	hit_s			= (hit_t * bdp) * ldp - bp1_along_line; // Is between 0 and ldp.sqr_length() if hit
	if (hit_s > 0 && hit_s < line_sq_len) report_hit_event(hit_type, hit_event(hit_t, nnorm));
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
			if (bounce_detected) {
				if (it_dt == next_bounce._t * t_left) break; // Further iterations will not improve the result
				else                                  it_dt = next_bounce._t * t_left;
			}
			else                 break;
		} // curr_bounce_iteration
		move_and_rotate_ball(it_dt);
		calculate_ball_acceleration();
		if (iterate_each_bounce) apply_ball_acceleration(it_dt, .5);
		if (bounce_detected) bounce_ball_vel_and_ang_vel(t_left); // Moves ball
		t_left -= it_dt;
	} // num_calls_left
	if (t_left != 0) { // Had to break because ball bounced to much (it's possibly rolling)
#if 0 // Anti-freeze measure method 1 ()
		time_taken -= t_left;
		lev.set_ball_vel(bv + .4*rotated_90_deg_cw(bv)); // Rotate velocity a litte bit to make the ball get out of the dead lock
#else // Anti-freeze measure method 1 (calculate the rest of the step without iterating each bounces)
		if (iterate_each_bounce) { // This will cause the ball to freeze when rolling since it's bouncing infinitely much
			for (int i = 0; i < NUM_SUB_STEPS_ON_FAILURE; i++) { // Divide the time that's left on NUM_SUB_STEPS_ON_FAILURE new steps, without bounce iteration
				step_dividing(t_left/NUM_SUB_STEPS_ON_FAILURE, false); // Shouldn't make the ball freeze because of rolling
			}
		}
		else { // To prevent even deeper recursion
			lev.set_ball_vel(vec(0,0)); // Stop ball, maybe it has got stuck
			time_taken -= t_left; // This would be unexpected
		}
#endif
	}
	if (!iterate_each_bounce) apply_ball_acceleration(dt, .5);
}

void physics::init_level_simulation() {
	// Simulation statistics
	time_taken				= 0;
	goal_reached			= false;
	// Physicalic start values
	ball_rad				= lev.get_square_scale()*lev.get_ball_scale()/2;
	ball_square				= lev.cannon_coords();
	lev.set_ball_pos(negated_y(vec(ball_square) + vec(0.5, 0.5))*lev.get_square_scale());
	lev.set_ball_vel(vec(lev.get_cannon()->_shot_vec) * CANNON_STRENGH);
	lev.set_ball_ang(0);
	lev.set_ball_ang_vel(0);
	// Simulation optins
	speed_factor			= 1;
	friction_coefficient	= FRICTION_COEFFICIENT;
	ball_moment_of_inertia	= 2./5*square(ball_rad) * BALL_MASS; // [kg*l.u.^2] (for mass = 1, see http://en.wikipedia.org/wiki/List_of_moments_of_inertia)
	max_negated_momentum	= MAX_NEGATED_MOMENTUM;
	bounce_coefficient		= BOUNCE_COEFFICIENT;
}

void physics::step(double dt) {
	// Re-calculate time step
	dt *= speed_factor;
	if (-dt > time_taken) dt = -time_taken;
	time_taken += dt;
	// Update position, velocity and acceleration of the ball
	step_dividing(dt);
	// Check for goal hit
	if (in_goal_this_step) {
		goal_reached = true;
		set_speed_factor(0.15);
	}
}

bool physics::has_reached_goal() {
	return goal_reached;
}

void physics::set_speed_factor(double f) {
	speed_factor = f;
}