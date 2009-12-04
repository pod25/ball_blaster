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
bool level::remove_obj(obj_coords coords) {
	return remove_obj(coords.x, coords.y, coords.i);
}
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

	// Allow only one cannon
	if(dynamic_cast<cannon*>(obj) && cannon_exists())
		remove_obj(cannon_coords());

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
		vvvobj::iterator x = _objects.begin();
		for(uint i = 0; i < w; i++) {
			if(x != _objects.end()) {
				while(x->size() > h)
					x->pop_back();
				x++;
			}
		}
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
 * Cannon exists?
 */
bool level::cannon_exists() {
	// Find cannon
	vvvobj::iterator	c;
	vvobj::iterator		r;
	vobj::iterator		ind;
	size_t x;
	size_t y;
	size_t i;
	for(c = _objects.begin(), x = 0; c != _objects.end(); c++, x++) {
		for(r = c->begin(), y = 0; r != c->end(); r++, y++) {
			for(ind = r->begin(), i = 0; ind != r->end(); ind++, i++) {
				if(dynamic_cast<cannon*>(*ind))
					return true;
			}
		}
	}
	return false;
}

/*
 * Get cannon position
 */
obj_coords level::cannon_coords() {
	// Find cannon
	vvvobj::iterator	c;
	vvobj::iterator		r;
	vobj::iterator		ind;
	int x;
	int y;
	int i;
	for(c = _objects.begin(), x = 0; c != _objects.end(); c++, x++) {
		for(r = c->begin(), y = 0; r != c->end(); r++, y++) {
			for(ind = r->begin(), i = 0; ind != r->end(); ind++, i++) {
				if(dynamic_cast<cannon*>(*ind))
					return obj_coords(x, y, i);
			}
		}
	}
	return obj_coords(0, 0, 0);
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
		obj_out.push_back(PROP_LEVEL_SIZE);
		obj_out.push_back(to_string(get_width()));
		obj_out.push_back(to_string(get_height()));
		obj_out.push_back(PROP_LEVEL_SQUARE_SCALE);
		obj_out.push_back(to_string(get_square_scale()));
		obj_out.push_back(PROP_LEVEL_BALL_SCALE);
		obj_out.push_back(to_string(get_ball_scale()));

		out.push_back(implode(obj_out, ','));
	}

	// Iterate all objects
	int i_x = 0;
	for(vvvobj::iterator x = _objects.begin(); x != _objects.end(); x++) {
		int i_y = 0;
		for(vvobj::iterator y = x->begin(); y != x->end(); y++) {
			for(vobj::iterator i = y->begin() ; i != y->end(); i++) {
				vector<string> obj_out;
				/* Wall object */ {
					wall* o = dynamic_cast<wall*>(*i);
					if(o) {
						obj_out.push_back(ID_WALL);
					}
				}

				/* Goal object */ {
					goal* o = dynamic_cast<goal*>(*i);
					if(o) {
						obj_out.push_back(ID_GOAL);
					}
				}

				/* Cannon object */ {
					cannon* o = dynamic_cast<cannon*>(*i);
					if(o) {
						obj_out.push_back(ID_CANNON);
					}
				}

				/* Magnet object */ {
					magnet* o = dynamic_cast<magnet*>(*i);
					if(o) {
						obj_out.push_back(ID_MAGNET);
						obj_out.push_back(PROP_DIR);
						obj_out.push_back(to_string(o->_dir));
						obj_out.push_back(PROP_STRENGTH);
						obj_out.push_back(to_string(o->_strength));
					}
				}
				
				/* Fan object */ {
					fan* o = dynamic_cast<fan*>(*i);
					if(o) {
						obj_out.push_back(ID_FAN);
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

	// Read file
	string content_string;
	string part;
	while(*file >> part)
		content_string.append(part);
	if(!content_string.length())
		return false;

	// Reset the active level
	new_level(name);

	// Split up into objects
	vector<string> objects = explode(content_string, '|');
	bool found_level = false;
	for(vector<string>::iterator i = objects.begin(); i != objects.end(); i++) {
		// Split up object string
		vector<string> object = explode(*i, ',');
		if(object.size() > 0) {
			// Get object id
			string id = object.front();
			object.erase(object.begin());

			vec		prop_gravity			(0, 0);
			int		prop_pos_x				= 0;
			int		prop_pos_y				= 0;
			int		prop_dir				= 0;
			int		prop_strength			= 0;
			int		prop_lev_size_x			= 0;
			int		prop_lev_size_y			= 0;
			int		prop_lev_grid_size		= 0;
			double	prop_lev_square_scale	= 0;
			double	prop_lev_ball_scale		= 0;

			// Load the properties
			while(object.size() > 0) {
				string prop = object.front();
				object.erase(object.begin());
				if(prop == PROP_GRAVITY) {
					prop_gravity.x = to_double(object[0]);
					prop_gravity.y = to_double(object[1]);
					object.erase(object.begin(), object.begin() + 2);
				}
				else if(prop == PROP_POS) {
					prop_pos_x = to_int(object[0]);
					prop_pos_y = to_int(object[1]);
					object.erase(object.begin(), object.begin() + 2);
				}
				else if(prop == PROP_DIR) {
					prop_dir = to_int(object[0]);
					object.erase(object.begin(), object.begin() + 1);
				}
				else if(prop == PROP_STRENGTH) {
					prop_strength = to_int(object[0]);
					object.erase(object.begin(), object.begin() + 1);
				}
				else if(prop == PROP_LEVEL_SIZE) {
					prop_lev_size_x = to_int(object[0]);
					prop_lev_size_y = to_int(object[1]);
					object.erase(object.begin(), object.begin() + 2);
				}
				else if(prop == PROP_LEVEL_GRID_SIZE) {
					prop_lev_grid_size = to_int(object[0]);
					object.erase(object.begin(), object.begin() + 1);
				}
				else if(prop == PROP_LEVEL_SQUARE_SCALE) {
					prop_lev_square_scale = to_double(object[0]);
					object.erase(object.begin(), object.begin() + 1);
				}
				else if(prop == PROP_LEVEL_BALL_SCALE) {
					prop_lev_ball_scale = to_double(object[0]);
					object.erase(object.begin(), object.begin() + 1);
				}
			}

			// Create the object
			if(id == ID_LEVEL) {
				set_gravity		(prop_gravity);
				set_size		(prop_lev_size_x, prop_lev_size_y);
				set_grid_size	(prop_lev_grid_size);
				set_square_scale(prop_lev_square_scale);
				set_ball_scale	(prop_lev_ball_scale);
				found_level		= true;
			}
			else if(id == ID_WALL) {
				wall* o = new wall(true);
				insert_obj(prop_pos_x, prop_pos_y, o);
			}
			else if(id == ID_GOAL) {
				goal* o = new goal(true);
				insert_obj(prop_pos_x, prop_pos_y, o);
			}
			else if(id == ID_CANNON) {
				cannon* o = new cannon();
				insert_obj(prop_pos_x, prop_pos_y, o);
			}
			else if(id == ID_MAGNET) {
				magnet* o = new magnet(true, prop_dir, prop_strength);
				insert_obj(prop_pos_x, prop_pos_y, o);
			}
			else if(id == ID_FAN) {
				fan* o = new fan(true, prop_dir, prop_strength);
				insert_obj(prop_pos_x, prop_pos_y, o);
			}
		}
	}
	if(!found_level)
		return false;

	return true;
}

/*
 * Reset the level and set level name
 */
bool level::new_level(string name) {
	_objects.clear();
	_level_name		= name;
	set_size		(LEVEL_DEFAULT_WIDTH, LEVEL_DEFAULT_HEIGHT);
	set_grid_size	(LEVEL_DEFAULT_GRID_SIZE);
	set_gravity		(vec(0, LEVEL_DEFAULT_GRAVITY));
	set_square_scale(1);
	set_ball_scale	(1);

	return false;
}
/*************************************************/