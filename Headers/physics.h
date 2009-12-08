/*
 * File: physics.h
 */
#ifndef PHYSICS_H
#define PHYSICS_H

/*
 * physics class
 */
class physics {
	vec ball_acceleration;
public:
	void init_level_simulation();
	void step(double dt);
};

#endif
/* end physics.h */