/*
 * File: objects.h
 */
#ifndef OBJECTS_H
#define OBJECTS_H

/*
 * object class
 */
class object abstract {
	friend	level;
protected:
	vec		_pos;
	bool	_locked;
public:
	virtual void test(); // TODO: Remove this muddafukka (needed for class to be polymorphic)
};

/*
 * directed object class
 */
class directed_object : public object {
	friend	level;
protected:
	int		dir;
};

/*
 * magnet class
 */
class magnet : public directed_object {

};

/*
 * nondirected object class
 */
class nondirected_object : public object {

};

/*
 * wall class
 */
class wall : public nondirected_object {

};

/*
 * goal class
 */
class goal : public nondirected_object {

};

#endif
/* end objects.h */