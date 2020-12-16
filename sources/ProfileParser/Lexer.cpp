/*
** G13, 2020
** ProfileParser / Lexer.cpp
*/

#include "ProfileParser/Lexer.hpp"
#include <algorithm>
#include <cassert>
#include <cctype>
#include <cstring>

////////////////////////////////////////////////////////////////////////////////

Lexer::Lexer(std::string_view source)
: GenericLexer(source)
, m_currentToken(Token::Type::Eof, std::string_view())
{}

////////////////////////////////////////////////////////////////////////////////

Token Lexer::next()
{
	while (true) {
		ignoreWhile(isspace);
		if (nextIs("//"))
			ignoreUntil('\n');
		else
			break;
	}

	auto type = Token::Type::Invalid;
	std::string_view value;

	if (nextIs(isalpha)) {
		value = consumeWhile(isalnum);

		auto it = s_keywords.find(value);
		if (it != s_keywords.end())
			type = it->second;
		else if (s_logitechKeys.count(value) > 0)
			type = Token::Type::GKeyLiteral;
		else if (s_pageNames.count(value) > 0)
			type = Token::Type::PageLiteral;
		else
			type = Token::Type::KeyLiteral;
	}
	else if (nextIs(isdigit)) {
		value = consumeWhile(isdigit);
		type = Token::Type::NumericLiteral;
	}
	else if (nextIs('"')) {
		auto quoted_string = consumeQuotedString();

		if (quoted_string.has_value()) {
			type = Token::Type::StringLiteral;
			value = quoted_string.value();
		}
		else {
			type = Token::Type::UnterminatedStringLiteral;
			syntaxError("Unterminated string literal");
		}
	}
	else if (isEof()) {
		type = Token::Type::Eof;
	}
	else {
		auto it = std::find_if(s_tokens.begin(), s_tokens.end(), [this] (auto pair) {
			return nextIs(pair.first);
		});
		if (it == s_tokens.end())
			syntaxError("Illegal character");
		else {
			type = it->second;
			value = it->first;
			ignore(it->first.length());
		}
	}

	m_currentToken = Token(type, value);
	return m_currentToken;
}

////////////////////////////////////////////////////////////////////////////////

const std::map<std::string_view, Token::Type> Lexer::s_keywords {
	{"bind", Token::Type::Bind},
	{"color", Token::Type::Color},
	{"page", Token::Type::Page},
	{"profile", Token::Type::Profile}
};

const std::set<std::string_view> Lexer::s_logitechKeys = {
	"G1",
	"G2",
	"G3",
	"G4",
	"G5",
	"G6",
	"G7",
	"G8",
	"G9",
	"G10",
	"G11",
	"G12",
	"G13",
	"G14",
	"G15",
	"G16",
	"G17",
	"G18",
	"G19",
	"G20",
	"G21",
	"G22",
	"Left",
	"Top",
	"Bottom",
	"Joystick",
	"JoystickUp",
	"JoystickRight",
	"JoystickDown",
	"JoystickLeft"
};

const std::set<std::string_view> Lexer::s_pageNames = {
	"M1",
	"M2",
	"M3"
};

const std::unordered_map<std::string_view, Token::Type> Lexer::s_tokens {
	{"+", Token::Type::Plus},
	{",", Token::Type::Comma},
	{";", Token::Type::SemiColon},
	{"{", Token::Type::BracketOpen},
	{"}", Token::Type::BracketClose},
};