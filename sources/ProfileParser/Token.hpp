/*
** G13, 2020
** ProfileParser / Token.hpp
*/

/*
 * Checklist:
 * - Token types
 * - Tokens
 * - Lexer
 * - Parser
 * - ProfileConfig?
 */

#pragma once

////////////////////////////////////////////////////////////////////////////////

#include <string_view>

////////////////////////////////////////////////////////////////////////////////

struct Token
{
	enum class Type
	{
		Invalid,
		Eof,

		BracketClose,
		BracketOpen,
		Comma,
		Plus,
		SemiColon,

		Bind,
		Color,
		Page,
		Profile,

		GKeyLiteral,
		NumericLiteral,
		KeyLiteral,
		PageLiteral,
		StringLiteral,
		UnterminatedStringLiteral,
	};

	Token(Type t, std::string_view v)
	: type(t)
	, value(v)
	{}

	const char* name() const { return name(type); }
	static const char* name(Type);

	Type type = Type::Invalid;
	std::string_view value;
};