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


	vec(double x_init = 0, double y_init = 0)
		: x(x_init), y(y_init) {}

	vec(const vec& existing)
		: x(existing.x), y(existing.y) {}

	vec&	operator=(const vec&);
	vec&	operator+=(const vec&);
	vec&	operator-=(const vec&);
	vec&	operator*=(const double);
	vec&	operator/=(const double);

	vec		operator+(const vec&);
	vec		operator-(const vec&);
	double	operator*(const vec&);
	double	operator%(const vec&);
	vec		operator*(const double);
	vec		operator/(const double);

	
	double length();
	double sqr_length();
	vec normalized();

};

vec operator*(double, const vec&);

#endif
/* end vec.h */