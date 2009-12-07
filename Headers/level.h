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
	vvvobj		_objects;		// Level object container
	size_t		_w;				// Number of squares in width
	size_t		_h;				// Number of squares in height
	uint		_grid_size;		// Visual square size in pixels
	double		_square_scale;	// Physical square scale
	double		_ball_scale;	// Ball diameter vs. square side ratio
	vec			_gravity;		// Gravity force vector 
	ball		_ball;			// The ball
	string		_level_name;	// The name of the current level
	
public:
	// Default values
	static const int LEVEL_MAX_WIDTH			= 500;
	static const int LEVEL_MAX_HEIGHT			= 500;
	static const int LEVEL_MIN_GRID_SIZE		= 16;
	static const int LEVEL_MAX_GRID_SIZE		= 64;

	static const int LEVEL_DEFAULT_WIDTH		= 20;
	static const int LEVEL_DEFAULT_HEIGHT		= 15;
	static const int LEVEL_DEFAULT_GRID_SIZE	= 32;
	static const int LEVEL_DEFAULT_GRAVITY		= 10;

	static const int MAGNET_DEFAULT_STRENGTH	= 10;

				level();
	size_t		num_objects			(size_t x, size_t y);
	object*		get_object			(size_t x, size_t y, size_t index);
	bool		remove_obj			(size_t x, size_t y, size_t index);
	bool		remove_obj			(obj_coords coords);
	bool		remove_obj_at_pixel	(uint pixel_x, uint pixel_y, bool can_edit_locked);
	bool		insert_obj			(size_t x, size_t y, object* obj);
	bool		insert_obj_at_pixel	(uint oc, uint pixel_x, uint pixel_y, bool locked);
	bool		can_insert_obj		(size_t x, size_t y, bool directed, uint direction = 0);
	size_t		get_width			();
	size_t		get_height			();
	uint		get_pixel_width		();
	uint		get_pixel_height	();
	bool		set_size			(size_t w, size_t h);
	uint		get_grid_size		();
	bool		set_grid_size		(uint size);
	double		get_square_scale	();
	bool		set_square_scale	(double scale);
	double		get_ball_scale		();
	bool		set_ball_scale		(double scale);
	vec			get_gravity			();	
	bool		set_gravity			(vec gravity);
	bool		set_ball_pos		(vec pos);
	vec			get_ball_pos		();
	bool		set_ball_vel		(vec vel);
	vec			get_ball_vel		();
	bool		ball_visible		();
	void		show_ball			();
	void		hide_ball			();
	bool		cannon_exists		();
	obj_coords	cannon_coords		();
	coords		vector_coords_from_pixel(int pixel_x, int pixel_y);
	coords		vector_coords_from_pixel(coords pixel);
	coords		pixel_coords_from_vector(uint vector_x, uint vector_y);
	coords		pixel_coords_from_vector(coords vector);
	uint		dir_from_pixel		(uint pixel_x, uint pixel_y);
	bool		is_wall				(size_t x, size_t y);

	bool		save_level		(string name);
	bool		load_level		(string name);
	bool		new_level		(string name);
private:
	level(const level&); //Protect from using copy constructor (until desired)
};

#endif
/* end level.h */