/*
 * File: physics.h
 */
#ifndef PHYSICS_H
#define PHYSICS_H

enum HIT_TYPES {
	HIT_BOUNCE,
	HIT_GOAL
};

/*
 * bounce_event class
 */
class hit_event {
public:
	double	_t;		// The time the bounce occured in the sub frame in the interval [0, 1]
	vec		_h_normal;	// The normal for the bouncing surface

	hit_event(double t, vec h_normal)
		: _t(t), _h_normal(h_normal) {}
	hit_event() {}
};

/*
 * physics class
 */
class physics {
	static const uint	MAX_FRAME_ITERATIONS = 50;

	// Simulation statistics
	double time_taken;
	// Physics constants
	double		bounce_coefficient;
	// Ball statistics
	double		ball_rad;
	vec			ball_acc;
	// Bounce event statistics
	bool		bounce_detected;
	hit_event	next_bounce;
	// Goal statistics
	bool		goal_reached;
	bool		in_goal_this_step;
	hit_event	next_goal_hit;

	void calculate_ball_acceleration();
	void apply_ball_acceleration	(double dt, double amount);
	void bounce_ball				(double& dt);
	void report_hit_event			(int hit_type, hit_event he);
	void hit_test_line				(int hit_type, vec lp1, vec dlp, vec bp1, vec bdp);
	void hit_test_circle			(int hit_type, vec cp, double crad, vec bp1, vec bdp, bool will_bounce = true);
	void hit_test_obj				(int hit_type, vec ul_crnr_pos, vec bp1, vec bdp);
	void step						(double dt, uint num_calls_left);
public:
	void init_level_simulation		();
	void step						(double dt);

	physics() {}
};
static const double BOUNCE_COEFFICIENT	= 0.8;
static const double CANNON_STRENGH		= 1.0;

#endif
/* end physics.h */