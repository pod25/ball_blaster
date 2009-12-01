/*
 * File: ball.h
 */
#ifndef BALL_H
#define BALL_H

/*
 * ball class
 */
class ball {
	friend	level;
private:
	vec		_pos;
	vec		_vel;
	bool	_visible;
};

#endif
/* end ball.h */