/*
 * File: objects.h
 */
#ifndef OBJECTS_H
#define OBJECTS_H

// Object identifiers
enum OBJECT_CLASSES {
	OC_WALL,
	OC_GOAL,
	OC_CANNON,
	OC_FAN,
	OC_MAGNET,
	NUM_OBJECT_CLASSES
};

// Object directions
#define DIR_NODIR 0
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
 * object class declaration
 */
class object {
	friend	level;
private:
	object();
protected:
	bool _locked;
public:
	object(bool locked) : _locked(locked) {};
	virtual ~object() {};
};

/*
 * directed object class declaration
 */
class directed_object : public object {
	friend	level;
private:
	directed_object();
protected:
	int	_dir;
public:
	directed_object(bool locked, int dir) : object(locked), _dir(min(dir, NUM_DIRECTIONS-1)) {};
	int	get_dir() { return _dir; };
};

/*
 * nondirected object class declaration
 */
class nondirected_object : public object {
	friend level;
private:
	nondirected_object();
public:
	nondirected_object(bool locked) : object(locked) {};
};

/*
 * magnet class declaration
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
 * fan class declaration
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
 * wall class declaration
 */
class wall : public nondirected_object {
	friend level;
public:
	wall() : nondirected_object(true) {};
};

/*
 * goal class declaration
 */
class goal : public nondirected_object {
	friend level;
public:
	goal() : nondirected_object(true) {};
};

/*
 * cannon class declaration
 */
class cannon : public nondirected_object {
	friend level;
public:
	cannon() : nondirected_object(true) {};
	vec _shot_vec;
};

#endif
/* end objects.h */