/*
 * File: menu_event_handler.cpp
 */
#include "common.h"

/*
 * Constructor
 */
menu_event_handler::menu_event_handler() {
	_clear_list();
	_init_main_menu();
}

/*
 * Menu mouse movement handler
 */
void menu_event_handler::e_mouse_move(int mouse_x, int mouse_y) {

}

/*
 * Menu mouse button handlers
 */
void menu_event_handler::e_mouse_down(int mouse_x, int mouse_y, int button) {

}
void menu_event_handler::e_mouse_up(int mouse_x, int mouse_y, int button) {

}

/*
 * Menu key button handlers
 */
void menu_event_handler::e_key_down(int key) {
	switch(_state) {
		case STATE_MAIN:
			if(key == SDLK_RETURN) {
				if(_list[_selection] == MENU_MAIN_QUIT)
					gam.quit();
				else if(_list[_selection] == MENU_MAIN_PLAY)
					_init_play_menu();
				else if(_list[_selection] == MENU_MAIN_EDIT)
					_init_edit_menu();
			}
			break;
		case STATE_PLAY:
			if(key == SDLK_RETURN) {
				if(_list[_selection] == MENU_BACK)
					_init_main_menu();
				else {
					string sel_level = _list[_selection];
					sel_level = sel_level.substr(2);
					lev.load_level(sel_level);
					cur_eh = &editor_eh;
				}
			}
		case STATE_EDIT:
			if(key == SDLK_RETURN) {
				if(_list[_selection] == MENU_BACK)
					_init_main_menu();
			}
	}
}
void menu_event_handler::e_key_up(int key) {
	switch(_state) {
		case STATE_MAIN:
		case STATE_PLAY:
		case STATE_EDIT:
			if(key == SDLK_UP)
				_dec_selection();
			else if(key == SDLK_DOWN)
				_inc_selection();
			break;
	}
}

/*
 * Menu new frame handler
 */
void menu_event_handler::e_new_frame() {
	gra.menu_background_buffer.apply(0, 0);

	// Display list
	image text;
	for(uint i = 0; i < _list.size(); i++) {
		SDL_Color* color = &gra.menu_color;
		if(i == _selection)
			color = &gra.menu_color_selected;
		text.generate_text(_list[i], gra.menu_font, *color);
		text.apply(LIST_X_POS, LIST_Y_POS + i * LIST_SPACING);
	}

	gra.update();
}

/*
 * Menu step handler
 */
void menu_event_handler::e_step(int delta_t) {

}

/*
 * Initialize menus
 */
void menu_event_handler::_init_main_menu() {
	_state = STATE_MAIN;
	_clear_list();
	_add_list_item(MENU_MAIN_PLAY);
	_add_list_item(MENU_MAIN_EDIT);
	_add_list_item(MENU_MAIN_QUIT);
}
void menu_event_handler::_init_play_menu() {
	_state = STATE_PLAY;
	_clear_list();
	vstring levels = get_level_file_list();
	for(int i = 0; i < (int)levels.size(); i++) {
		_add_list_item(levels[i].insert(0, "- "));
	}
	_add_list_item(MENU_BACK);
}
void menu_event_handler::_init_edit_menu() {
	_state = STATE_EDIT;
	_clear_list();
	_add_list_item(MENU_BACK);
}

/*
 * Selection list functions
 */
void menu_event_handler::_clear_list() {
	_list.clear();
	_selection = 0;
}
void menu_event_handler::_add_list_item(string str) {
	_list.push_back(str);
}
void menu_event_handler::_inc_selection() {
	_selection++;
	if(_selection >= (int)_list.size())
		_selection = 0;
}
void menu_event_handler::_dec_selection() {
	_selection--;
	if(_selection < 0)
		_selection = max(0, (int)_list.size() - 1);
}