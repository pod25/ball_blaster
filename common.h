/*
 * File: common.h
 */
#ifndef COMMON_H
#define COMMON_H

/*
 * Library includes
 */
#include <string>

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
typedef SDL_Surface		sdl_surface;
typedef std::string		str;

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