/*
 * File: level.h
 */
#ifndef LEVEL_H
#define LEVEL_H

/*
 * level class
 */
class level {
private:
	vvobj		_objects;		// Level object container
	size_t		_w;				// Number of squares in width
	size_t		_h;				// Number of squares in height
	uint		_grid_size;		// Visual square size in pixels
	double		_square_scale;	// Physical square scale
	double		_ball_scale;	// Ball diameter vs. square side ratio
	vec			_gravity;		// Gravity force vector 
	ball		_ball;			// The ball
	string		_level_name;	// The name of the current level
	
	size_t		_index_from_coord(size_t x, size_t y);
public:
	size_t		num_objects		(size_t x, size_t y);
	object*		get_object		(size_t x, size_t y, size_t index);
	bool		remove_obj		(size_t x, size_t y, size_t index);
	bool		insert_obj		(size_t x, size_t y, object* obj);
	size_t		get_width		();
	size_t		get_height		();
	bool		set_size		(size_t w, size_t h);
	uint		get_grid_size	();
	bool		set_grid_size	(uint size);
	double		get_square_scale();
	bool		set_square_scale(double scale);
	double		get_ball_scale	();
	bool		set_ball_scale	(double scale);
	vec			get_gravity		();	
	bool		set_gravity		(vec gravity);
	bool		set_ball_pos	(vec pos);
	vec			get_ball_pos	();
	bool		set_ball_vel	(vec vel);
	vec			get_ball_vel	();
	bool		ball_visible	();
	void		show_ball		();
	void		hide_ball		();
	void		save_level		(string name);
	void		load_level		(string name);
	void		new_level		(string name);

	level() {}; // Default constructor
private:
	level(const level&); //Protect from using copy constructor (until desired)
};

#endif
/* end level.h */