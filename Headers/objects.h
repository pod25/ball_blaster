/*
 * File: objects.h
 */
#ifndef OBJECTS_H
#define OBJECTS_H

// Object identifiers
enum OBJECT_CLASSES {
	OC_WALL,
	OC_GOAL_AREA,
	OC_CANNON,
	OC_FAN,
	OC_MAGNET,
	NUM_OBJECT_CLASSES
};

// Object directions
enum OBJECT_DIRECTIONS {
	DIR_LEFT,
	DIR_UP,
	DIR_RIGHT,
	DIR_DOWN,
	NUM_DIRECTIONS
};

#define ID_LEVEL				"LEVEL"
#define ID_WALL					"WALL"
#define ID_GOAL					"GOAL"
#define ID_MAGNET				"MAGNET"
#define ID_FAN					"FAN"
#define ID_CANNON				"CANNON"
#define PROP_GRAVITY			"GRAVITY"
#define PROP_POS				"POS"
#define PROP_DIR				"DIR"
#define PROP_STRENGTH			"STRENGTH"
#define PROP_LEVEL_SIZE			"LEVEL_SIZE"
#define PROP_LEVEL_GRID_SIZE	"GRID_SIZE"
#define PROP_LEVEL_SQUARE_SCALE	"SQUARE_SCALE"
#define PROP_LEVEL_BALL_SCALE	"BALL_SCALE"

/*
 * object class
 */
class object {
	friend	level;
private:
	object();
protected:
	bool	_locked;
public:
	object(bool locked) : _locked(locked) {};
	virtual ~object() {};
};

/*
 * directed object class
 */
class directed_object : public object {
	friend	level;
private:
	directed_object();
protected:
	int		_dir;
public:
	directed_object(bool locked, int dir) : object(locked), _dir(dir) {};
};

/*
 * nondirected object class
 */
class nondirected_object : public object {
	friend level;
private:
	nondirected_object();
public:
	nondirected_object(bool locked) : object(locked) {};
};

/*
 * magnet class
 */
class magnet : public directed_object {
	friend level;
private:
	int _strength;
	magnet();
public:
	magnet(bool locked, int dir, int strength) : directed_object(locked, dir), _strength(strength) {};
};

/*
 * fan class
 */
class fan : public directed_object {
	friend level;
private:
	int _strength;
	fan();
public:
	fan(bool locked, int dir, int strength) : directed_object(locked, dir), _strength(strength) {};
};

/*
 * wall class
 */
class wall : public nondirected_object {
	friend level;
private:
	wall();
public:
	wall(bool locked) : nondirected_object(locked) {};
};

/*
 * goal class
 */
class goal : public nondirected_object {
	friend level;
private:
	goal();
public:
	goal(bool locked) : nondirected_object(locked) {};
};

/*
 * cannon class
 */
class cannon : public nondirected_object {
	friend level;
public:
	cannon() : nondirected_object(true) {};
};

#endif
/* end objects.h */