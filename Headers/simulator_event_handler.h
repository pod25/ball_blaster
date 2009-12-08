/*
 * File: simulator_event_handler.h
 */
#ifndef SIMULATOR_EVENT_HANDLER_H
#define SIMULATOR_EVENT_HANDLER_H

/*
 * simulator_event_handler class
 */
class simulator_event_handler : public event_handler {
	enum SIMULATION_STATES {
		STATE_NOT_STARTED,
		STATE_RUNNING,
		STATE_FINISHED
	};
	static const int	FOLLOW_LIMIT = 100;

	SIMULATION_STATES	curr_state;
	bool				_from_editor;
	coords				_window_pos;
	void _follow_ball	();
	void _plot_square	(size_t x, size_t y);
public:
	simulator_event_handler();
	void init				(bool from_editor);
	void e_mouse_move		(int mouse_x, int mouse_y);
	void e_mouse_down		(int mouse_x, int mouse_y, int button);
	void e_mouse_up			(int mouse_x, int mouse_y, int button);
	void e_key_down			(int key);
	void e_key_up			(int key);
	void e_new_frame		();
	void e_step				(int delta_t);
	void refresh_obj_layer	();
};

#endif
/* end simulator_event_handler.h */