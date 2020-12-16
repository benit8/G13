/*
** G13, 2019
** Main.cpp
*/

#include "Manager.hpp"
#include "Common/Exception.hpp"
#include "Common/Logger.hpp"

#include "ProfileParser/Lexer.hpp"
#include "WM/WM.hpp"

int main(/*int argc, char const* argv[]*/)
{
#if 1
	WM::the().dumpKeycodes();

	try {
		G13::Manager manager;
		return manager.run();
	}
	catch (Exception& e) {
		Logger::fatal(e.what());
	}
#else
	const char* source = "profile \"Default\" {\n\tpage M1 {\n\t\tcolor 100,0,0;\n\t\tbind G1 Escape;\n\t\tbind G3 Ctrl+Z;\n\t\tbind G5 F5;\n\t\tbind G7 Ctrl+Alt+T;\n\t\tbind G10 Ctrl+X;\n\t\tbind G11 Ctrl+C;\n\t\tbind G12 Ctrl+V;\n\t\tbind G22 Space;\n\t\tbind Left \"Random copy pasta\";\n\t}\n}";
	Lexer l(source);
	auto t = l.next();
	while (t.type != Token::Type::Eof && !l.hasErrors()) {
		std::cout << '[' << t.name() << ':' << t.value << ']' << ' ' << std::endl;
		t = l.next();
	}
#endif

	return EXIT_FAILURE;
}