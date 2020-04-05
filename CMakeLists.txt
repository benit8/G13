##
## G13, 2019
## CMakeLists.txt
##

cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

project(G13 VERSION 0.1.0)
add_executable(g13d)

target_compile_features(g13d
PUBLIC
	cxx_std_17
)

target_compile_options(g13d
PUBLIC
	-fPIC
	-W -Wall -Wextra
	-g3
)

target_include_directories(g13d
PUBLIC
	.
	src
)

target_sources(g13d
PUBLIC
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

target_link_libraries(g13d
PUBLIC
	usb-1.0
)