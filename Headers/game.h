/*
 * File: game.h
 */
#ifndef GAME_H
#define GAME_H

/*
 * game class declaration
 */
class game {
private:
	coords				_window_pos;
	bool				_quit;
	uint				_speed_multiplyer;
public:
	static const int	FPS = 50;
	static const int	MAX_STEP_DELTA_MS = 20;

	void				init_game					();
	void				main						();
	void				quit						();
	void				set_speed_multiplyer		(uint m = 1);
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