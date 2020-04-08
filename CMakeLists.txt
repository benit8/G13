##
## G13, 2019
## CMakeLists.txt
##

cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)
cmake_policy(SET CMP0076 NEW)

project(G13 VERSION 0.1.0 LANGUAGES CXX C)
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
	sources
)

target_sources(g13d
PUBLIC
	extern/tinyformat.hpp
	extern/font8x8/get_character.cpp

	sources/Main.cpp
	sources/Exception.hpp
	sources/Logger.hpp
	sources/Logger.cpp
	sources/RenderTarget.hpp
	sources/RenderTarget.cpp
	sources/G13/Manager.hpp
	sources/G13/Manager.cpp
	sources/G13/Keys.hpp
	sources/G13/Device.hpp
	sources/G13/Device.cpp
	sources/G13/Display.hpp
	sources/G13/Display.cpp
	sources/G13/Profile.hpp
	sources/G13/Profile.cpp
)

target_link_libraries(g13d
PUBLIC
	usb-1.0
)