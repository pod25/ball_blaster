/*
 * File: physics.h
 */
#ifndef PHYSICS_H
#define PHYSICS_H

enum HIT_TYPES {
	HIT_BOUNCE,
	HIT_GOAL,
	HIT_CHANGE_SQUARE
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
	static const uint	BALL_MASS						= 1;
	static const uint	MAX_MOVES_PER_STEP				= 10;
	static const uint	NUM_BOUNCES_ITERATIONS			= 5;
	static const uint	NUM_SUB_STEPS_ON_FAILURE		= 10;
	static const uint	MAX_MOVES_PER_STEP_ON_FAILURE	= 10;

	// Simulation statistics
	double		time_taken;
	double		speed_factor;
	// Physics constants
	double		bounce_coefficient;
	double		max_negated_momentum;
	// Ball statistics
	double		ball_rad;
	vec			ball_acc;
	coords		ball_square;
	double		ball_moment_of_inertia;
	double		friction_coefficient;
	// Bounce event statistics
	bool		bounce_detected;
	hit_event	next_bounce;
	// Goal statistics
	bool		goal_reached;
	bool		in_goal_this_step;
	hit_event	next_goal_hit;

	void calculate_ball_acceleration();
	void apply_ball_acceleration	(double dt, double amount);
	void move_and_rotate_ball		(double dt);
	void bounce_ball_vel_and_ang_vel(double dt);
	void report_hit_event			(int hit_type, hit_event he);
	void hit_test_line				(int hit_type, vec lp1, vec ldp, vec bp1, vec bdp);
	void hit_test_circle			(int hit_type, vec cp, double crad, vec bp1, vec bdp);
	void hit_test_obj				(int hit_type, vec ul_crnr_pos, vec bp1, vec bdp);
	void step_dividing				(double dt, bool iterate_each_bounce = true);
public:
	void init_level_simulation		();
	void step						(double dt);
	bool has_reached_goal			();
	void set_speed_factor			(double f);

	physics() {}
};
static const double FRICTION_COEFFICIENT	= 0.2;
static const double MAX_NEGATED_MOMENTUM	= 0.4;
static const double BOUNCE_COEFFICIENT		= 0.8;
static const double CANNON_STRENGH			= 1.0;

#endif
/* end physics.h */