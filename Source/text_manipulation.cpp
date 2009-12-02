/*
 * File: text_manipulation.cpp
 */
#include "common.h"

/*
 * Trim spaces from start and end of string
 */
string trim(string text) {
	// Removes initial spaces
	while(text.substr(0, 1) == " ")
		text = text.substr(1);
	// Removes subsequent spaces after final character
	while(text.substr(text.length() - 1, 1) == " ")
		text = text.substr(0, text.length() - 1);

	return text;
}

/*
 * Splits up a string into a vector using given delimiter character
 */
vector<string> explode(string text, const char delimiter) {
	vector<string> result;
	uint found;
	// Index of first delimiter
	found = text.find_first_of(delimiter);
	while(found != string::npos) {
		// Checks that delimiter is not first entry and that text exists
		if(found > 0 && trim(text.substr(0,found)) != "")
			// Inserts trimmed part of string into result
			result.push_back(trim(text.substr(0,found)));
		text = text.substr(found+1);
		found = text.find_first_of(delimiter);
	}
	if(text.length() > 0) {
		if(trim(text.substr(0,found)) != "")
			result.push_back(trim(text));
	}

	return result;
}

/*
 * Combines elements in a vector to a string separated with given delimiter
 */
string implode(vector<string> text, const char delimiter) {
	string result;
	for(uint i = 0; i < text.size(); i++) {
		result += text[i];
		if(i < (text.size()-1))
			result += delimiter;
	}

	return result;
}

/*
 * Convert values to string
 */
string to_string(int value) {
	string result;
	std::stringstream out;
	out << value;
	result = out.str();

	return result;
}
string to_string(double value) {
	string result;
	std::stringstream out;
	out << value;
	result = out.str();

	return result;
}