/*
 * File: obj_coords.h
 */

#ifndef OBJ_COORDS_H
#define OBJ_COORDS_H

class obj_coords {
public:
	size_t x;
	size_t y;
	size_t i;
	obj_coords(size_t in_x, size_t in_y, size_t in_i) : x(in_x), y(in_y), i(in_i) {};
	obj_coords() : x(0), y(0), i(0) {};
	obj_coords(coords c) : x(c.x), y(c.y), i(0) {};
};

#endif
/* end obj_coords.h */