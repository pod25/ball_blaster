/*
 * File: vec.cpp
 */
#include "common.h"

vec& vec::operator=(const vec& rhs) {
	x = rhs.x;
	y = rhs.y;
	return *this;
}

vec& vec::operator+=(const vec& rhs) {
	x += rhs.x;
	y += rhs.y;
	return *this;
}

vec& vec::operator-=(const vec& rhs) {
	x -= rhs.x;
	y -= rhs.y;
	return *this;
}

vec& vec::operator*=(const double multiplier) {
	x *= multiplier;
	y *= multiplier;
	return *this;
}

vec& vec::operator/=(const double divider) {
	if (!divider) throw domain_error("Trying to divide a vector by zero");
	x /= divider;
	y /= divider;
	return *this;
}

vec vec::operator+(const vec& rhs) {
	return vec(x+rhs.x, y+rhs.y);
}

vec vec::operator-(const vec& rhs) {
	return vec(x-rhs.x, y-rhs.y);
}

double vec::operator*(const vec& rhs) {
	return (x*rhs.x + y*rhs.y);
}

double vec::operator%(const vec& rhs) {
	return (x*rhs.y - y*rhs.x);
}

vec vec::operator*(const double multiplier) {
	return vec(multiplier*x, multiplier*y);
}

vec vec::operator/(const double divider) {
	if(!divider) throw domain_error("Not defined to divide with zero");
	return vec(x/divider, y/divider);
}

double vec::length() {
	return sqrt(sqr_length());
}

double vec::sqr_length() {
	return (x*x + y*y);
}

vec vec::normalized() {
	double len = length();
	if (!len) throw domain_error("Trying to normalize a zero-length vec");
	return vec(x/len, y/len);
}

void vec::normalize() {
	double len = length();
	if (!len) throw domain_error("Trying to normalize a zero-length vec");
	x /= len;
	y /= len;
}

vec operator*(double lhs, const vec& rhs) {
	return vec(lhs*rhs.x, lhs*rhs.y);
}

vec negated_y(const vec& v) {
	return vec(v.x, -v.y);
}

vec rot_90_deg_ccw (const vec& v) {
	return vec(-v.y, v.x);
}

coords vec_to_coords (const vec& v) {
	return coords(int(v.x), int(v.y));
}