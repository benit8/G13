/*
** G13, 2020
** Common / GenericLexer.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

#include <functional>
#include <optional>
#include <string_view>

////////////////////////////////////////////////////////////////////////////////

class GenericLexer
{
public:
	explicit GenericLexer(std::string_view input);
	virtual ~GenericLexer() = default;

	using Condition = std::function<bool(char)>;

	size_t tell() const { return m_index; }
	size_t tellRemaining() const { return m_input.length() - m_index; }

	bool isEof() const;

	char peek(size_t offset = 0) const;

	bool nextIs(char expected) const;
	bool nextIs(std::string_view expected) const;
	bool nextIs(Condition) const;

	char consume();
	bool consumeSpecific(char specific);
	bool consumeSpecific(std::string_view str);
	std::string_view consume(size_t count);
	std::string_view consumeAll();
	std::string_view consumeLine();
	std::string_view consumeWhile(Condition);
	std::string_view consumeUntil(char stop);
	std::string_view consumeUntil(std::string_view stop);
	std::string_view consumeUntil(Condition);
	std::optional<std::string_view> consumeQuotedString();

	void ignore(size_t count = 1);
	void ignoreWhile(Condition);
	void ignoreUntil(char stop);
	void ignoreUntil(std::string_view stop);
	void ignoreUntil(Condition);

protected:
	std::string_view m_input;
	size_t m_index { 0 };
};