/*
 * File: level.cpp
 */
#include "common.h"

/*************************************************
 * PUBLIC FUNCTION DEFINITIONS
 *************************************************/ 
/*
 * Constructor
 */
level::level() {
	new_level("");
}

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
* Check for object wall in square
*/
bool level::is_wall(size_t x, size_t y) {
	if (dynamic_cast<wall*>(get_object(x, y, 0)))
		return true;
	else
		return false;
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
 * Get object pointer at obj_coords
 */
object* level::get_object(obj_coords coords) {
return get_object(coords.x, coords.y, coords.i);
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
		
		// Update object layer
		editor_eh.objects_changed(x, y);
		return true;
	}
	return false;
}
/*
 * Remove object at level pixel coordinates (pixel_x, pixel_y)
 */
bool level::remove_obj_at_pixel(uint pixel_x, uint pixel_y, bool can_edit_locked) {
	coords	vector_pos	= vector_coords_from_pixel(pixel_x, pixel_y);
	uint	dir			= dir_from_pixel(pixel_x, pixel_y);
	size_t	num_obj		= num_objects(vector_pos.x, vector_pos.y);
	for(size_t i = 0 ; i < num_obj; i++) {
		// Locked?
		object* obj = get_object(vector_pos.x, vector_pos.y, i);
		if(obj->_locked && !can_edit_locked)
			return false;

		// Directed object?
		directed_object* directed = dynamic_cast<directed_object*>(obj);
		if((directed && directed->get_dir() == dir) || directed == 0) {
			remove_obj(vector_pos.x, vector_pos.y, i);
			return true;
		}
	}
	return false;
}

/*
 * Insert object at vector coordinates (x, y)
 */
bool level::insert_obj(size_t x, size_t y, object* obj) {

	// Type casts the input object to directed object if that is the case
	directed_object* dir_obj = dynamic_cast<directed_object*>(obj);

	// Is object directed?
	bool is_dir		= (dir_obj == 0) ? false: true;
	uint direction	= (dir_obj == 0) ? false: dir_obj->_dir;
	
	// Insertion possible?
	if(!can_insert_obj(x, y, is_dir, direction))
		return false;

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

	// Update object layer
	editor_eh.objects_changed(x, y);

	return true;
}

/*
 * Insert object of type oc at level pixel coordinates (pixel_x, pixel_y)
 */
bool level::insert_obj_at_pixel(uint oc, uint pixel_x, uint pixel_y, bool locked) {
	coords	vector_pos	= vector_coords_from_pixel(pixel_x, pixel_y);
	uint	dir			= dir_from_pixel(pixel_x, pixel_y);
	object* o;
	switch(oc) {
	case OC_CANNON:
		o = new cannon();
		if(insert_obj(vector_pos.x, vector_pos.y, o))
			return true;
		break;
	case OC_WALL:
		o = new wall();
		if(insert_obj(vector_pos.x, vector_pos.y, o))
			return true;
		break;
	case OC_GOAL:
		o = new goal();
		if(insert_obj(vector_pos.x, vector_pos.y, o))
			return true;
		break;
	case OC_MAGNET:
		o = new magnet(locked, dir, MAGNET_DEFAULT_STRENGTH);
		if(insert_obj(vector_pos.x, vector_pos.y, o))
			return true;
		break;
	case OC_FAN:
		o = new fan(locked, dir, MAGNET_DEFAULT_STRENGTH);
		if(insert_obj(vector_pos.x, vector_pos.y, o))
			return true;
		break;
	}
	// Delete object in case the insertion went wrong
	delete o;
	return false;
}

/*
 * Can insert directed/nondirected object at vector pos (x, y) ?
 */
bool level::can_insert_obj(size_t x, size_t y, bool directed, uint direction) {
	// In bounds?
	if(x >= _w || y >= _h)
		return false;

	// Gets number of objects in selected square
	size_t num_obj = num_objects(x, y);

	// Checks when insert is forbidden
	if(num_obj == 1 && dynamic_cast<nondirected_object*>(get_object(x, y, 0)))
		return false;
	else if(!directed && num_obj > 0)
		return false;
	else if(directed) {
		for(size_t n = 0; n < num_obj; n++) {
			if(dynamic_cast<directed_object*>(get_object(x, y, n))) 
				if(dynamic_cast<directed_object*>(get_object(x, y, n))->_dir == direction)
					return false;
		}
		// Checks that directed object is inserted against wall

		if(direction == DIR_DOWN)
			if(!is_wall(x, (y - 1)))
				return false;
		if(direction == DIR_UP)
			if(!is_wall(x, (y + 1)))
				return false;
		if(direction == DIR_RIGHT)
			if(!is_wall((x - 1), y))
				return false;
		if(direction == DIR_LEFT)
			if(!is_wall((x + 1), y))
				return false;		
	}

	// Insertion allowed!
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
 * Get level width in pixels
 */
size_t level::get_pixel_width() {
	return _w * _grid_size;
}

/*
 * Get level height in pixels
 */
size_t level::get_pixel_height() {
	return _h * _grid_size;
}

/*
 * Set level size in squares
 */
bool level::set_size(size_t w, size_t h) {
	// New size in bound?
	if(w < ceil(1.0 * gra.SCREEN_WIDTH / get_grid_size()))
		w = (size_t)ceil(1.0 * gra.SCREEN_WIDTH / get_grid_size());
	if(w > lev.LEVEL_MAX_WIDTH)
		w = lev.LEVEL_MAX_WIDTH;
	if(h < ceil(1.0 * gra.SCREEN_HEIGHT / get_grid_size()))
		h = (size_t)ceil(1.0 * gra.SCREEN_HEIGHT / get_grid_size());
	if(h > lev.LEVEL_MAX_HEIGHT)
		h = lev.LEVEL_MAX_HEIGHT;

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

	// Update object layer
	gra.set_object_layer_size(get_pixel_width(), get_pixel_height());

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
	if(size < lev.LEVEL_MIN_GRID_SIZE)
		_grid_size = lev.LEVEL_MIN_GRID_SIZE;
	else if(size > lev.LEVEL_MAX_GRID_SIZE)
		_grid_size = lev.LEVEL_MAX_GRID_SIZE;
	else
		_grid_size = size;
	
	// Make sure that this change didn't make the level smaller than the window
	set_size(get_width(), get_height());

	// Resize image buffers
	gra.set_grid_size(get_grid_size());

	// Redraw object layer
	editor_eh.objects_changed(0, 0, true);

	return true;
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
 * Get ball size in pixels
 */
int level::get_ball_pixel_size() {
	return (int)(get_grid_size() * get_ball_scale());
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
 * Get vector coordinates from pixel coordinates relative to the level
 */
coords level::vector_coords_from_pixel(int pixel_x, int pixel_y) {
	return coords(pixel_x/get_grid_size(), pixel_y/get_grid_size());
}
coords level::vector_coords_from_pixel(coords pixel) {
	return vector_coords_from_pixel(pixel.x, pixel.y);
}

/*
 * Get pixel coordinates (relative to the level) from vector coordinates
 */
coords level::pixel_coords_from_vector(uint vector_x, uint vector_y) {
	return coords(vector_x*get_grid_size(), vector_y*get_grid_size());
}
coords level::pixel_coords_from_vector(coords vector) {
	return pixel_coords_from_vector(vector.x, vector.y);
}

/*
 * Get object direction from pixel coordinates relative to the level
 */
uint level::dir_from_pixel(uint pixel_x, uint pixel_y) {
	uint grid = get_grid_size();
	uint x_offset = pixel_x % grid;
	uint y_offset = pixel_y % grid;
	uint lowest_dist = x_offset;
	uint dir = DIR_RIGHT;
	if(y_offset < lowest_dist) {
		lowest_dist = y_offset;
		dir = DIR_DOWN;
	}
	if(grid - x_offset < lowest_dist) {
		lowest_dist = grid - x_offset;
		dir = DIR_LEFT;
	}
	if(grid - y_offset < lowest_dist)
		dir = DIR_UP;
	return dir;
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
				wall* o = new wall();
				insert_obj(prop_pos_x, prop_pos_y, o);
			}
			else if(id == ID_GOAL) {
				goal* o = new goal();
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