/*
 * File: menu_event_handler.h
 */
#ifndef MENU_EVENT_HANDLER_H
#define MENU_EVENT_HANDLER_H

#define MENU_MAIN_PLAY	"Play Level"
#define MENU_MAIN_EDIT	"Edit Level"
#define MENU_MAIN_QUIT	"Quit"
#define MENU_BACK		"Back"
#define MENU_NEW_LEVEL	"New Level"

/*
 * menu_event_handler class
 */
class menu_event_handler : public event_handler {
private:
	enum STATES {
		STATE_MAIN,
		STATE_PLAY,
		STATE_EDIT
	};
	static const int LIST_X_POS		= 100;
	static const int LIST_Y_POS		= 200;
	static const int LIST_SPACING	= 40;
	uint			_state;
	vstring			_list;
	int				_selection;

	void _init_main_menu();
	void _init_play_menu();
	void _init_edit_menu();
	void _clear_list();
	void _add_list_item(string str);
	void _inc_selection();
	void _dec_selection();
public:
	menu_event_handler();
	void e_mouse_move	(int mouse_x, int mouse_y);
	void e_mouse_down	(int mouse_x, int mouse_y, int button);
	void e_mouse_up		(int mouse_x, int mouse_y, int button);
	void e_key_down		(int key);
	void e_key_up		(int key);
	void e_new_frame	();
	void e_step			(int delta_t);
	void menu_reset		();
};

#endif
/* end menu_event_handler.h */