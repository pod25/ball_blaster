/*
 * File: objects.cpp
 */
#include "common.h"

object::object(bool locked) {
	_locked = locked;
}

directed_object::directed_object(bool locked, int dir) : object(locked) {
	_dir = dir;
}

nondirected_object::nondirected_object(bool locked) : object(locked) {
}

magnet::magnet(bool locked, int dir, int strength) : directed_object(locked, dir) {
	_strength = strength;
}

wall::wall(bool locked) : nondirected_object(locked) {
}

goal::goal(bool locked) : nondirected_object(locked) {
}