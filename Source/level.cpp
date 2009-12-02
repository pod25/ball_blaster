/*
 * File: level.cpp
 */
#include "common.h"

/*************************************************
 * PUBLIC FUNCTION DEFINITIONS
 *************************************************/ 
/*
 * Get number of objects at (x, y)
 */
size_t level::num_objects(size_t x, size_t y) {
	if(x < _objects.size())
		if(y < _objects[x].size())
			return _objects[x][y].size();
	return 0;
}

/*
 * Get object pointer at (x, y, index)
 */
object*	level::get_object(size_t x, size_t y, size_t index) {
	// Checks that object exists
	if(index < num_objects(x, y))
		return _objects[x][y][index];
	else
		return 0;
}

/*
 * Remove object at (x, y, index)
 */
bool level::remove_obj(size_t x, size_t y, size_t index) {
	// Checks that object exists
	if(index < num_objects(x, y)) {
		_objects[x][y].erase(_objects[x][y].begin() + index);
		return true;
	}
	return false;
}	

/*
 * Insert object at (x, y)
 */
bool level::insert_obj(size_t x, size_t y, object* obj) {
	// In bounds?
	if(x >= _w || y >= _h)
		return false;

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
			if(dynamic_cast<directed_object*>(get_object(x, y, n))->_dir == dir_obj->_dir)
				return false;
		}
	}
	// Inserts object into desired coordinate.
	if(x >= _objects.size()) {
		_objects.resize(x+1);
	}
	if(y >= _objects[x].size()) {
		_objects[x].resize(y+1);
	}

	_objects[x][y].push_back(obj);

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
 * Set level size in squares
 */
bool level::set_size(size_t w, size_t h) {
	// Checks if level size has been reduced
	if(w < _w || h < _h) {
		while(_objects.size() > w)
			_objects.pop_back();
		for(vvvobj::iterator x = _objects.begin(); x < _objects.begin() + w && _objects.size() > 0; x++)
			while(x->size() > h)
				x->pop_back();
	}
	_w = w;
	_h = h;
	return true;
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
	_ball._vel = vel;
	return true;
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
	_ball._visible = true;
}

/*
 * Hide the ball
 */
void level::hide_ball() {
	_ball._visible = false;
}

/*
 * Save the level to 'Levels/{name}.lev'
 */
bool level::save_level(string name) {
	vector<string> out;
	
	/* Level properties */ {
		vector<string> obj_out;
		obj_out.push_back(ID_LEVEL);
		obj_out.push_back(PROP_GRAVITY);
		obj_out.push_back(to_string(get_gravity().x));
		obj_out.push_back(to_string(get_gravity().y));
		out.push_back(implode(obj_out, ','));
	}

	// Iterate all objects
	int i_x = 0;
	for(vvvobj::iterator x = _objects.begin(); x != _objects.end(); x++) {
		int i_y = 0;
		for(vvobj::iterator y = x->begin(); y != x->end(); y++) {
			for(vobj::iterator i = y->begin() ; i != y->end(); i++) {
				vector<string> obj_out;
				/* Wall object*/ {
					wall* o = dynamic_cast<wall*>(*i);
					if(o) {
						obj_out.push_back(ID_WALL);
					}
				}

				/* Magnet object*/ {
					magnet* o = dynamic_cast<magnet*>(*i);
					if(o) {
						obj_out.push_back(ID_MAGNET);
						obj_out.push_back(PROP_DIR);
						obj_out.push_back(to_string(o->_dir));
						obj_out.push_back(PROP_STRENGTH);
						obj_out.push_back(to_string(o->_strength));
					}
				}

				// Position is common to all objects
				obj_out.push_back(PROP_POS);
				obj_out.push_back(to_string(i_x));
				obj_out.push_back(to_string(i_y));

				// Add object string to out-vector
				out.push_back(implode(obj_out, ','));
			}
			i_y++;
		}
		i_x++;
	}

	// Generate file string and save
	if(save_level_file(name, implode(out,'|'))) {
		// If successful set current level name
		_level_name = name;
		return true;
	}
	return false;
}

/*
 * Load a level from 'Levels/{name}.lev'
 */
bool level::load_level(string name) {
	// Open file
	ifstream* file = open_level_file(name);
	if(!file->is_open())
		return false;

	int		prop_pos_x		= 0;
	int		prop_pos_y		= 0;
	double	prop_gravity_x	= 0;
	int		prop_gravity_y	= 0;
	int		prop_dir		= 0;
	int		prop_strength	= 0;

	// Read file
	string content_string;
	string part;
	while(*file >> part)
		content_string.append(part);

	// CONTINUE HERE

	return false;
}

/*
 * Reset the level and set level name
 */
bool level::new_level(string name) {
	return false;
}
/*************************************************/