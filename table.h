#include <iostream>
#include <cstring>
#include <cctype>
#include <fstream>
#include "website.h"

class table {
	public:
		table(); // default constructor
		table(const table &); // copy constructor
		~table(); // class destructor

		bool add(const website &); // this will call the private function to hash
		bool getSitesByTopic(const char *) // this will call private method to search the lists


	private:
};
