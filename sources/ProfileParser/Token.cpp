/*
** G13, 2020
** ProfileParser / Token.cpp
*/

#include "ProfileParser/Token.hpp"
#include <cassert>

////////////////////////////////////////////////////////////////////////////////

const char *Token::name(Token::Type type)
{
	switch (type) {
		case Type::Invalid: return "Invalid";
		case Type::Eof: return "Eof";

		case Type::BracketClose: return "BracketClose";
		case Type::BracketOpen: return "BracketOpen";
		case Type::Comma: return "Comma";
		case Type::Plus: return "Plus";
		case Type::SemiColon: return "SemiColon";

		case Type::Bind: return "Bind";
		case Type::Color: return "Color";
		case Type::Page: return "Page";
		case Type::Profile: return "Profile";

		case Type::GKeyLiteral: return "GKeyLiteral";
		case Type::NumericLiteral: return "NumericLiteral";
		case Type::KeyLiteral: return "KeyLiteral";
		case Type::PageLiteral: return "PageLiteral";
		case Type::StringLiteral: return "StringLiteral";
		case Type::UnterminatedStringLiteral: return "UnterminatedStringLiteral";
	}

	assert(false);
}