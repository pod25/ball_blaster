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
	return level_name_to_file_name(name.insert(0,"Levels\\"));
}

/*
 * Get level file list
 */
vstring get_level_file_list() {
	vstring list;
	WIN32_FIND_DATA file_image;
	HANDLE file_handle = FindFirstFile("Levels/*.lev", &file_image);

	bool working = true;
	if(file_handle != INVALID_HANDLE_VALUE) {
		string buffer = file_image.cFileName;
		list.push_back(file_name_to_level_name(buffer));
		while(working) {
			FindNextFile(file_handle, &file_image);
			if(file_image.cFileName != buffer) {
				buffer = file_image.cFileName;
				list.push_back(file_name_to_level_name(buffer));
			}
			else {
				//end of files reached
				working=false;
			}
		}
	}

	return list;
}