/*
 * File: physics.h
 */
#ifndef PHYSICS_H
#define PHYSICS_H

/*
 * bounce_event class
 */
class bounce_event {
public:
	double	_t;		// The time the bounce occured in the sub frame in the interval [0, 1]
	vec		_b_normal;	// The normal for the bouncing surface
	bool	_goal;

	bounce_event(double t, vec b_normal, bool goal = false)
		: _t(t), _b_normal(b_normal), _goal(goal) {}
	bounce_event() {}
};

/*
 * physics class
 */
class physics {
	static const uint	MAX_FRAME_ITERATIONS = 50;

	// Ball statistics
	double			ball_rad;
	vec				ball_acc;
	// Bounce event statistics
	bool			bounce_detected;
	bounce_event	next_bounce;
	// Goal statistics
	bool			goal_reached;

	void calculate_ball_acceleration();
	void apply_ball_acceleration	(double dt, double amount);
	bool bounce_check_line			(vec lp1, vec dlp, vec bp1, vec bdp);
	bool bounce_check_circle		(vec cp, double crad, vec bp1, vec bdp, bool will_bounce = true);
	bool bounce_check_obj			(vec ul_crnr_pos, vec bp1, vec bdp);
	void bounce_ball				(double& dt);
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