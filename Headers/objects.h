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

#define ID_LEVEL		"LEVEL"
#define ID_WALL			"WALL"
#define ID_GOAL			"GOAL"
#define ID_MAGNET		"MAGNET"
#define PROP_GRAVITY	"GRAVITY"
#define PROP_POS		"POS"
#define PROP_DIR		"DIR"
#define PROP_STRENGTH	"STRENGTH"

/*
 * object class
 */
class object {
	friend	level;
protected:
	bool	_locked;
public:
	virtual void test(){}; // TODO: Remove this muddafukka (needed for class to be polymorphic)
};

/*
 * directed object class
 */
class directed_object : public object {
	friend	level;
protected:
	int		_dir;
};

/*
 * magnet class
 */
class magnet : public directed_object {
	friend level;
private:
	int		_strength;
};

/*
 * nondirected object class
 */
class nondirected_object : public object {
	friend level;
};

/*
 * wall class
 */
class wall : public nondirected_object {
	friend level;
};

/*
 * goal class
 */
class goal : public nondirected_object {
	friend level;
};

#endif
/* end objects.h */