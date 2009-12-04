/*
 * File: coords.h
 */

#ifndef COORDS_H
#define COORDS_H

class coords {
public:
	int x;
	int y;
	coords(int in_x, int in_y) : x(in_x), y(in_y) {};
	coords() : x(0), y(0) {};
};

#endif
/* end coords.h */