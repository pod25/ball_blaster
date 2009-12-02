/*
 * File: text_manipulation.h
 */
#ifndef TEXT_MANIPULATION_H
#define TEXT_MANIPULATION_H

// Function declarations
string			trim	(const string& text);
vector<string>	explode	(const string& text, const char delimiter);
string			implode	(const string& text, const char delimiter);

string			to_string(int);
string			to_string(double);

#endif