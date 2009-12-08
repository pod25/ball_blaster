/*
 * File: vec.h
 */
#ifndef VEC_H
#define VEC_H

/*
 * vec struct
 */
class vec {
public:
	double		x;
	double		y;

	vec&	operator=(const vec&);
	vec&	operator+=(const vec&);
	vec&	operator-=(const vec&);
	vec&	operator*=(const double);
	vec&	operator/=(const double);

	vec		operator+(const vec&);
	vec		operator-(const vec&);
	double	operator*(const vec&); // Dot product, scalar product
	double	operator%(const vec&); // Determinant of corresponding 2x2 matrix
	vec		operator*(const double);
	vec		operator/(const double);

	double length();
	double sqr_length();
	vec normalized();
	void normalize();

	vec() : x(0), y(0) {} // Default constructor
	vec(const vec& existing) // Copy constructor
		: x(existing.x), y(existing.y) {}
	vec(double x_init, double y_init)
		: x(x_init), y(y_init) {}
	vec(coords c)
		: x(c.x), y(c.y) {}
	vec(obj_coords c)
		: x(c.x), y(c.y) {}
	~vec() {} // Default destructor
};

vec operator*(double, const vec&);

#endif
/* end vec.h */