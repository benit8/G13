##
## EPITECH PROJECT, 2019
## G13
## File description:
## CMakeLists.txt
##

cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

project(G13)

add_definitions("-std=c++17")
add_definitions("-W -Wall -Wextra")
add_definitions("-g3")

set(SOURCES
	extern/tinyformat.hpp

	src/Main.cpp
	src/Exception.hpp
	src/Logger.hpp
	src/Logger.cpp
	src/Manager.hpp
	src/Manager.cpp
	src/Device.hpp
	src/Device.cpp
	src/LCDDisplay.hpp
	src/LCDDisplay.cpp
)

add_executable(g13d ${SOURCES})
target_include_directories(g13d PUBLIC src/ extern/)
target_link_libraries(g13d usb-1.0)