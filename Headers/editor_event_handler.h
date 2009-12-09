/*
 * File: editor_event_handler.h
 */
#ifndef EDITOR_EVENT_HANDLER_H
#define EDITOR_EVENT_HANDLER_H

/*
 * editor_event_handler class
 */
class editor_event_handler : public event_handler {
private:
	enum STATES {
		STATE_DEFAULT,
		STATE_CANNON_CONFIG,
		STATE_INSERTION
	};

	static const uint SCROLL_AREA_SIZE	= 25;
	static const uint MAX_SCROLL_SPEED	= 200; // In pixels/second

	int		_mouse_x;
	int		_mouse_y;
	bool	_left_mouse_is_down;
	bool	_right_mouse_is_down;
	int		_state;
	bool	_can_edit_const;
	int		_sel_obj_type;
	bool	_objects_changed;
	bool	_scrolled;
	vec		_floating_window_pos;

	bool	_is_const_type(uint oc);
	bool	_is_dir_type(uint oc);
	void	_plot_square(size_t x, size_t y);
	double	_scroll_distance(int mouse_offset, uint delta_t);
	coords	_mouse_coords();
public:
	editor_event_handler();

	void e_mouse_move		(int mouse_x, int mouse_y);
	void e_mouse_down		(int mouse_x, int mouse_y, int button);
	void e_mouse_up			(int mouse_x, int mouse_y, int button);
	void e_key_down			(int key);
	void e_key_up			(int key);
	void e_new_frame		();
	void e_step				(int delta_t);
	void objects_changed	(size_t x, size_t y, bool all = false);
	bool start_simulation	();
	void set_mode			(bool constant);
	bool get_mode			();
	void reset_state		();
};

#endif
/* end editor_event_handler.h */