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
	double	_time;
	vec		_b_pos;
	bool	_goal;
	bool operator<(const bounce_event& b) {return _time < b._time;}
	bounce_event(double time, vec b_pos, bool goal = false)
		: _time(time), _b_pos(b_pos), _goal(goal) {}
private:
	bounce_event();
};

/*
 * physics class
 */
class physics {
	static const uint	MAX_FRAME_ITERATIONS = 50;

	// Ball information
	double			ball_rad;
	vec				ball_acc;
	// Bounce event information
	bool			bounce_detected;
	bounce_event	next_bounce();

	void calculate_ball_acceleration();
	void apply_ball_acceleration	(double dt, double amount);
	void bounce_check_obj			(object* curr_obj, vec p1, vec v);
	void bounce_ball				();
	void step						(double dt, uint num_calls_left);
public:
	void init_level_simulation	();
	void step					(double dt);
};
static const double BOUNCE_COEFFICIENT	= 0.8;
static const double CANNON_STRENGH		= 1.0;

#endif
/* end physics.h */