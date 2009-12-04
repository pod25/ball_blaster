/*
 * File: game.h
 */
#ifndef GAME_H
#define GAME_H

/*
 * game class
 */
class game {
private:
	coords				window_pos;
public:
	static const int	FPS = 50;

	void				init_game		();
	void				main			();
	void				set_window_pos	(coords);
	coords				get_window_pos	();

};

#endif
/* end game.h */