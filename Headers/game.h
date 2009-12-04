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

	void				init_game					();
	void				main						();
	void				set_window_pos				(uint x, uint y);
	void				set_window_pos				(coords pos);
	coords				get_window_pos				();
	coords				level_pos_from_window_pos	(uint x, uint y);
	coords				level_pos_from_window_pos	(coords pos);
	coords				window_pos_from_level_pos	(uint x, uint y);
	coords				window_pos_from_level_pos	(coords pos);
};

#endif
/* end game.h */