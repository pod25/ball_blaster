/*
 * File: event_handler.h
 */
#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

/*
 * event_handler class declaration
 */
class event_handler abstract {
public:
	virtual void e_mouse_move	(int mouse_x, int mouse_y) = 0;
	virtual void e_mouse_down	(int mouse_x, int mouse_y, int button) = 0;
	virtual void e_mouse_up		(int mouse_x, int mouse_y, int button) = 0;
	virtual void e_key_down		(int key) = 0;
	virtual void e_key_up		(int key) = 0;
	virtual void e_new_frame	() = 0;
	virtual void e_step			(int delta_t) = 0; // delta_t is time (in ms) since last step
};

#endif
/* end event_handler.h */