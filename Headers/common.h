/*
 * File: common.h
 */
#ifndef COMMON_H
#define COMMON_H

/*
 * Library includes
 */
#include <string>
using std::string;
#include <vector>
using std::vector;

/*
 * SDL includes
 */
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"

/*
 * Class/Struct pre-declarations
 */
class event_handler;
class editor_event_handler;
class simulator_event_handler;

class ball;
class game;
class graphics;
class level;
class physics;

class object;
class directed_object;
class nondirected_object;
class magnet;
class wall;
class goal;

/*
 * Typedefs
 */
typedef SDL_Surface					sdl_surface;
typedef vector<vector<object*>>		vvobj;
typedef unsigned int				uint;
typedef uint						size_t;

/*
 * Global variables
 */
extern editor_event_handler		editor_eh;
extern simulator_event_handler	sim_eh;
extern event_handler*			cur_eh;
extern graphics					gra;
extern physics					phy;
extern level					lev;

/*
 * Ball Blaster includes
 */
#include "vec.h"
#include "ball.h"
#include "event_handler.h"
#include "editor_event_handler.h"
#include "simulator_event_handler.h"
#include "game.h"
#include "graphics.h"
#include "level.h"
#include "objects.h"
#include "physics.h"

#endif
/* end common.h */