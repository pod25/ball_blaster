/*
 * File: text_manipulation.h
 */
#ifndef TEXT_MANIPULATION_H
#define TEXT_MANIPULATION_H

// Function declarations
string			trim	(string text);
vector<string>	explode	(string text, const char delimiter);
string			implode	(vector<string> text, const char delimiter);

string			to_string	(size_t);
string			to_string	(int);
string			to_string	(double);
double			to_double	(string);
int				to_int		(string);

#endif