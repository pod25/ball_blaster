/*
 * File: text_manipulation.cpp
 */
#include "common.h"

/*
 * Trim spaces/tabs and linebreaks from start and end of string
 */
string trim(string text) {
	while(text.substr(0, 1) == " ")
		text = text.substr(1);
	while(text.substr(text.length() - 1, 1) == " ")
		text = text.substr(0, text.length() - 1);

	return text;
}

/*
 * Splits up a string into a vector using given delimiter character
 */
vector<string> explode(const string& text, const char delimiter) {
	vector<string> result;

	return result;
}

/*
 * Combines elements in a vector to a string separated with given delimiter
 */
string implode(const string& text, const char delimiter) {
	string result;

	return result;
}

/*
 * Convert values to string
 */
string to_string(int) {
	string result;

	return result;
}
string to_string(double) {
	string result;

	return result;
}