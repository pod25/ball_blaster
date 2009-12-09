/*
 * File: physics.h
 */
#ifndef PHYSICS_H
#define PHYSICS_H

/*
 * physics class
 */
class physics {
	//static const double	BOUNCE_COEFFICIENT;
	static const uint	MAX_FRAME_ITERATIONS = 50;

	double	ball_rad;
	vec		ball_acc;

	void calculate_ball_acceleration();
	void apply_ball_acceleration	(double dt, double amount);
	void step						(double dt, uint num_calls_left);
public:
	void init_level_simulation	();
	void step					(double dt);
};

static const double BOUNCE_COEFFICIENT	= 0.8;
static const double CANNON_STRENGH		= 1.0;

#endif
/* end physics.h */