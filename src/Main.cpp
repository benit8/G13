/*
** G13, 2019
** Main.cpp
*/

#include "Manager.hpp"
#include "Exception.hpp"
#include "Logger.hpp"

int main(/*int argc, char const *argv[]*/)
{
	try {
		G13::Manager manager;
		return manager.run();
	}
	catch (G13::Exception &e) {
		Logger::fatal(e.what());
	}

	return EXIT_FAILURE;
}