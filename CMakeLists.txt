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
	extern/font-5x8.h

	sources/Color.hpp
	sources/Exception.hpp
	sources/Logger.cpp
	sources/Logger.hpp
	sources/Main.cpp
	sources/RenderTarget.cpp
	sources/RenderTarget.hpp

	sources/G13/Actions.hpp
	sources/G13/Device.cpp
	sources/G13/Device.hpp
	sources/G13/Display.cpp
	sources/G13/Display.hpp
	sources/G13/Keys.hpp
	sources/G13/Manager.cpp
	sources/G13/Manager.hpp
	sources/G13/Profile.cpp
	sources/G13/Profile.hpp
)

target_link_libraries(g13d
PUBLIC
	usb-1.0
)