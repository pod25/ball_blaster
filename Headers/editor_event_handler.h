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
	int _mouse_x;
	int _mouse_y;
public:
	editor_event_handler();

	void e_mouse_move	(int mouse_x, int mouse_y);
	void e_mouse_down	(int mouse_x, int mouse_y, int button);
	void e_mouse_up		(int mouse_x, int mouse_y, int button);
	void e_key_down		(int key);
	void e_key_up		(int key);
	void e_new_frame	();
	void e_step			(int delta_t);
};

#endif
/* end editor_event_handler.h */