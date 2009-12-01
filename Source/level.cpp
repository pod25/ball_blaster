/*
 * File: level.cpp
 */
#include "common.h"

/*************************************************
 * PRIVATE FUNCTION DEFINITIONS
 *************************************************/ 
size_t level::_index_from_coord(size_t x, size_t y) {
	return y * _w + x;
}
/*************************************************/

/*************************************************
 * PUBLIC FUNCTION DEFINITIONS
 *************************************************/ 
/*
 * Get number of objects at (x, y)
 */
size_t level::num_objects(size_t x, size_t y) {
	return _objects[_index_from_coord(x, y)].size();
}

/*
 * Get object pointer at (x, y, index)
 */
object*	level::get_object(size_t x, size_t y, size_t index) {
	// Checks that object exists
	if(index < num_objects(x, y))
		return _objects[_index_from_coord(x, y)][index];
	else
		return 0;
}

/*
 * Remove object at (x, y, index)
 */
bool level::remove_obj(size_t x, size_t y, size_t index) {
	// Checks that object exists
	if(index < num_objects(x, y)) {
		vector<object*>* cur_vec = &_objects[_index_from_coord(x, y)];
		cur_vec->erase(cur_vec->begin() + index);
		return true;
	}
	return false;
}	

/*
 * Insert object at (x, y)
 */
bool level::insert_obj(size_t x, size_t y, object* obj) {
	// Type casts the input object to directed object if that is the case
	directed_object* dir_obj = dynamic_cast<directed_object*>(obj);

	// Is object directed?
	bool in_dir = (dir_obj == 0) ? false: true;
	// Gets number of objects in selected square
	size_t num_obj = num_objects(x, y);

	// Checks when insert is forbidden
	if(num_obj == 1 && dynamic_cast<directed_object*>(get_object(x, y, 0)))
		return false;
	else if(!in_dir && num_obj > 0)
		return false;
	else if(in_dir) {
		for(size_t n = 0; n < num_obj; n++) {
			if(dynamic_cast<directed_object*>(get_object(x, y, n))->dir == dir_obj->dir)
				return false;
		}
	}
	// Inserts object into desired coordinate.
	_objects[_index_from_coord(x, y)].push_back(obj);
	return true;
}

/*
 * Get level width in squares
 */
size_t level::get_width() {
	return _w;
}

/*
 * Get level height in squares
 */
size_t level::get_height() {
	return _h;
}

/*
 * Get level size in squares
 */
bool level::set_size(size_t w, size_t h) {
	// Come back!!  Loop to find and store objects!
	_w = w;
	_h = h;
	return false;
}

/*
 * Get visual grid size (in pixels)
 */
uint level::get_grid_size() {
	return _grid_size;
}

/*
 * Set visual grid size (in pixels)
 */
bool level::set_grid_size(uint size) {
	if(size > 0) {
		_grid_size = size;
		return true;
	}
	else
		return false;
}

/*
 * Get square scale factor (physical)
 */
double level::get_square_scale() {
	return _square_scale;
}

/*
 * Set square scale factor (physical)
 */
bool level::set_square_scale(double scale) {
	if(scale > 0) {
		_square_scale = scale;
		return true;
	}
	else
		return false;
}

/*
 * Get ball scale factor (visual and physical)
 */
double level::get_ball_scale() {
	return _ball_scale;
}

/*
 * Set ball scale factor (visual and physical)
 */
bool level::set_ball_scale(double scale) {
	if(scale > 0) {
		_ball_scale = scale;
		return true;
	}
	else
		return false;
}

/*
 * Get gravity force vector
 */
vec	level::get_gravity() {
	return _gravity;
}

/*
 * Set gravity force vector
 */
bool level::set_gravity(vec gravity) {
	_gravity = gravity;
	return true;
}

/*
 * Get ball position vector
 */
vec level::get_ball_pos() {
	return _ball._pos;
}

/*
 * Set ball position to vector
 */
bool level::set_ball_pos(vec pos) {
	if(pos.x >= 0 && pos.y >= 0)
		_ball._pos = pos;
	return true;
}

/*
 * Get ball velocity vector
 */
vec level::get_ball_vel() {
	return _ball._vel;
}

/*
 * Set ball velocity to vector
 */
bool level::set_ball_vel(vec vel) {
	return false;
}

/*
 * Is the ball visible?
 */
bool level::ball_visible() {
	return _ball._visible;
}

/*
 * Show the ball
 */
void level::show_ball() {
}

/*
 * Hide the ball
 */
void level::hide_ball() {
}

/*
 * Save the level to 'Levels/{name}.lev'
 */
void level::save_level(string name) {
}

/*
 * Load a level from 'Levels/{name}.lev'
 */
void level::load_level(string name) {
}

/*
 * Reset the level and set level name
 */
void level::new_level(string name) {
}
/*************************************************/