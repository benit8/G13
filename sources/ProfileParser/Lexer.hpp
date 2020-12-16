/*
** G13, 2020
** ProfileParser / Lexer.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

#include "Common/GenericLexer.hpp"
#include "ProfileParser/Token.hpp"

#include <map>
#include <set>
#include <string_view>
#include <unordered_map>

////////////////////////////////////////////////////////////////////////////////

class Lexer : public GenericLexer
{
public:
	explicit Lexer(std::string_view source);
	virtual ~Lexer() = default;

	bool hasErrors() const { return m_hasErrors; }
	Token next();

private:
	void syntaxError(const char* msg)
	{
		m_hasErrors = true;
		fprintf(stderr, "Syntax error: %s\n", msg);
	}

private:
	Token m_currentToken;
	bool m_hasErrors = false;

	static const std::map<std::string_view, Token::Type> s_keywords;
	static const std::set<std::string_view> s_logitechKeys;
	static const std::set<std::string_view> s_pageNames;
	static const std::unordered_map<std::string_view, Token::Type> s_tokens;
};