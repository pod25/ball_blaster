/*
 * File: common.h
 */
#ifndef COMMON_H
#define COMMON_H

/*
 * Library includes
 */
#include <stdio.h>
#include <string>
using std::string;
#include <vector>
using std::vector;
#include <fstream>
using std::ofstream;
using std::ifstream;
#include <sstream>
#include <stdexcept>
using std::exception;
using std::domain_error;
using std::invalid_argument;
#include <cmath>
#include <iostream> //For debugging
using std::endl;
using std::cout;
using std::cerr;
#include <windows.h>

/*
 * SDL includes
 */
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"

/*
 * Class/Struct pre-declarations
 */
class error_handler;
class event_handler;
class editor_event_handler;
class simulator_event_handler;

class ball;
class game;
class level;
class sdl_handler;
class graphics;
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
typedef vector<object*>						vobj;
typedef vector<vector<object*>>				vvobj;
typedef vector<vector<vector<object*>>>		vvvobj;
typedef unsigned int						uint;
typedef uint8_t								byte;
//The type size_t
#ifndef _SIZE_T_DEFINED
#define _SIZE_T_DEFINED
typedef unsigned int						size_t;
#endif


/*
 * Global variables
 */
extern error_handler			err_hndl_obj;
extern editor_event_handler		editor_eh;
extern simulator_event_handler	sim_eh;
extern event_handler*			cur_eh;
extern sdl_handler				sdl_obj;
extern graphics					gra;
extern physics					phy;
extern level					lev;
extern game						gam;

/*
 * Ball Blaster includes
 */
#include "error_handler.h"
#include "text_manipulation.h"
#include "coords.h"
#include "obj_coords.h"
#include "vec.h"
#include "ball.h"
#include "font.h"
#include "image.h"
#include "event_handler.h"
#include "editor_event_handler.h"
#include "simulator_event_handler.h"
#include "files.h"
#include "game.h"
#include "level.h"
#include "objects.h"
#include "sdl_handler.h"
#include "graphics.h"
#include "physics.h"

#endif
/* end common.h */