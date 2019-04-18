/*
** EPITECH PROJECT, 2019
** G13
** File description:
** Main.cpp
*/

#include "Exception.hpp"
#include "Logger.hpp"
#include "Manager.hpp"

int main(int /*argc*/, char const */*argv*/[])
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