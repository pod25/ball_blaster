/*
 * File: error_handler.h
 */
#ifndef ERROR_HANDLER_H
#define ERROR_HANDLER_H

/*
 * error_handler class declaration
 */
class error_handler {
public:
	void inform_about_error(string caller, exception &error, bool terminate = true) {
		cerr << "Unexpected error occured in " << caller << endl;
		cerr << "\tCaught: " << error.what() << endl;
		cerr << "\tType: " << typeid(error).name() << endl;
		MessageBox(0, ("Unexpected error occured in " + caller + "\n" + error.what()).c_str(),
			"Ball Blaster 2000", MB_ICONERROR);
		if (terminate) exit(1);
	}
};

#endif
/* end error_handler.h */