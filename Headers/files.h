/*
 * File: files.h
 */
#ifndef FILES_H
#define FILES_H

/*
 * File function declarations
 */
ifstream*	open_level_file(string name);
bool		save_level_file(string name, const string& content);
string		path_from_level_name(string name);
vstring		get_level_file_list();

#endif
/* end files.h */