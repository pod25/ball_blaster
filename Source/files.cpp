/*
 * File: files.cpp
 */
#include "common.h"

/*
 * Open level file at 'Level/{name}.lev and returns file handle
 */
ifstream* open_level_file(string name) {
	// Open file for reading only
	ifstream* file = new ifstream;
	file->open(path_from_level_name(name).c_str());
	return file;
}

/*
 * Save level contents (presumed to be formatted) to 'Level/{name}.lev'
 */
bool save_level_file(string name, const string& content) {
	string path = path_from_level_name(name);
	// Delete file if it already exists
	ifstream* existing = open_level_file(name);
	if(existing->is_open()) {
		existing->close();
		remove(path.c_str());
	}

	// Open new output file
	ofstream file;
	file.open(path.c_str());
	if(file.is_open()) {
		file << content;
		file.close();
		return true;
	}
	return false;
}

/*
 * Get path to level with given name
 */
string path_from_level_name(string name) {
	string path = "Levels\\";
	path.append(name);
	path.append(".lev");
	return path;
}