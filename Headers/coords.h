/*
 * File: coords.h
 */

#ifndef COORDS_H
#define COORDS_H

/*
 * coords class declaration
 */
class coords {
public:
	int x;
	int y;

	coords operator+(const coords& c)	{return coords(x+c.x, y+c.y);}
	coords operator-(const coords& c)	{return coords(x-c.x, y-c.y);}
	coords operator*(const int i)		{return coords(x*i  , y*i  );}
	coords operator/(const int i)		{return coords(x/i  , y/i  );}

	coords(int in_x, int in_y) : x(in_x), y(in_y) {};
	coords() : x(0), y(0) {};
	coords(obj_coords c);
};

#endif
/* end coords.h */