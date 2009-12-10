/*
 * File: ball.h
 */
#ifndef BALL_H
#define BALL_H

/*
 * ball class declaration
 */
class ball {
	friend	level;
private:
	vec		_pos;
	vec		_vel;
	double	_ang;
	double	_ang_vel;
	bool	_visible;
};

#endif
/* end ball.h */