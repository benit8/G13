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
	cxx_std_20
)

target_compile_options(g13d
PUBLIC
	-fPIC
	-W -Wall -Wextra
	-g3
)

target_include_directories(g13d
PRIVATE
	extern
	sources
)

target_sources(g13d
PRIVATE
	extern/tinyformat.hpp
	extern/font-5x8.h

	sources/Actions.hpp
	sources/Device.cpp
	sources/Device.hpp
	sources/Display.cpp
	sources/Display.hpp
	sources/Keys.hpp
	sources/main.cpp
	sources/Manager.cpp
	sources/Manager.hpp
	sources/Profile.cpp
	sources/Profile.hpp
	sources/ProfileParser/Token.cpp
	sources/ProfileParser/Token.hpp
	sources/ProfileParser/Lexer.cpp
	sources/ProfileParser/Lexer.hpp

	sources/Common/Color.hpp
	sources/Common/Exception.hpp
	sources/Common/GenericLexer.cpp
	sources/Common/GenericLexer.hpp
	sources/Common/Logger.cpp
	sources/Common/Logger.hpp
	sources/Common/RenderTarget.cpp
	sources/Common/RenderTarget.hpp

	sources/WM/WM.hpp
	sources/WM/X11/X11.hpp
	sources/WM/X11/X11.cpp
)

target_link_libraries(g13d
PUBLIC
	usb-1.0
	X11
)