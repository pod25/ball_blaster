/*
 * File: vec.cpp
 */
#include "common.h"
#include <cmath>


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
	x /= divider;
	y /= divider;
	return *this;
}

vec vec::operator+(const vec& rhs) {
	vec temp(x, y);
	return temp += rhs;
}

vec vec::operator-(const vec& rhs){
	vec temp(x, y);
	return temp -= rhs;
}

double vec::operator*(const vec& rhs) {
	return (x*rhs.x + y*rhs.y);
}

double vec::operator%(const vec& rhs) {
return (x*rhs.y - y*rhs.x);
}

vec vec::operator*(const double multiplier) {
	vec temp(x, y);
	return temp *= multiplier;
}
vec vec::operator/(const double divider) {
	vec temp(x, y);
	return temp /= divider;
}


double vec::length() {
	return sqrt(sqr_length());
}

double vec::sqr_length() {
	return (pow(x, 2) + pow(y,2));
}

vec vec::normalized() {
	vec temp(x, y);
	temp /= length();
	return temp;
}

vec operator*(double lhs, const vec& rhs) {
	vec temp(rhs);
	return temp *= lhs;
}

//