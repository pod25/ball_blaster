/*
 * File: coords.cpp
 */
#include "common.h"

coords::coords(obj_coords c) : x(c.x), y(c.y) {}

// Doesn't seem to wotk; a cast to vec is by some reason performed first and then the multiplication... Stupid MS VC
coords operator*(int i, coords c) {return coords(i*c.x, i*c.y);}