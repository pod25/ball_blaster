/*
 * File: objects.h
 */
#ifndef OBJECTS_H
#define OBJECTS_H

// Object identifiers
enum OBJECT_CLASSES {
	//OC_WALL,
	OC_GOAL_AREA,
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
#define PROP_GRAVITY			"GRAVITY"
#define PROP_POS				"POS"
#define PROP_DIR				"DIR"
#define PROP_STRENGTH			"STRENGTH"
#define PROP_LEVEL_SIZE			"LEVEL_SIZE"
#define PROP_LEVEL_GRID_SIZE	"GRID_SIZE"

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
	object(bool locked);
	virtual void test(){}; // TODO: Remove this muddafukka (needed for class to be polymorphic)
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
	directed_object(bool locked, int dir);
};

/*
 * nondirected object class
 */
class nondirected_object : public object {
	friend level;
private:
	nondirected_object();
public:
	nondirected_object(bool locked);
};

/*
 * magnet class
 */
class magnet : public directed_object {
	friend level;
private:
	magnet();
private:
	magnet(bool locked, int dir, int strength);
	int		_strength;
};

/*
 * wall class
 */
class wall : public nondirected_object {
	friend level;
private:
	wall();
public:
	wall(bool locked);
};

/*
 * goal class
 */
class goal : public nondirected_object {
	friend level;
private:
	goal();
public:
	goal(bool locked);
};

#endif
/* end objects.h */