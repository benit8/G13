/*
** G13, 2020
** Common / GenericLexer.cpp
*/

#include "Common/GenericLexer.hpp"
#include <algorithm>
#include <cassert>

////////////////////////////////////////////////////////////////////////////////

GenericLexer::GenericLexer(std::string_view input)
: m_input(input)
{}

////////////////////////////////////////////////////////////////////////////////

// Tells whether the parser's index has reached input's end
bool GenericLexer::isEof() const
{
	return m_index >= m_input.length();
}

// Returns the current character at the parser index, plus `offset` if specified
char GenericLexer::peek(size_t offset) const
{
	return (m_index + offset < m_input.length()) ? m_input[m_index + offset] : '\0';
}

// Tests the next character in the input
bool GenericLexer::nextIs(char expected) const
{
	return peek() == expected;
}

// Tests if the `expected` string comes next in the input
bool GenericLexer::nextIs(std::string_view expected) const
{
	for (size_t i = 0; i < expected.length(); ++i)
		if (peek(i) != expected[i])
			return false;
	return true;
}

// Tests the next character against a Condition
bool GenericLexer::nextIs(Condition condition) const
{
	return condition(peek());
}

// Consume a character and advance the parser index
char GenericLexer::consume()
{
	assert(!isEof());
	return m_input[m_index++];
}

// Consume the given character if it is next in the input
bool GenericLexer::consumeSpecific(char specific)
{
	if (peek() != specific)
		return false;

	ignore();
	return true;
}

// Consume the given string if it is next in the input
bool GenericLexer::consumeSpecific(std::string_view str)
{
	if (!nextIs(str))
		return false;

	ignore(str.length());
	return true;
}

// Consume a number of characters
std::string_view GenericLexer::consume(size_t count)
{
	if (count == 0)
		return {};

	size_t start = m_index;
	size_t length = std::min(count, m_input.length() - m_index);
	m_index += length;

	return m_input.substr(start, length);
}

// Consume the rest of the input
std::string_view GenericLexer::consumeAll()
{
	if (isEof())
		return {};

	auto rest = m_input.substr(m_index, m_input.length() - m_index);
	m_index = m_input.length();
	return rest;
}

// Consume until a new line is found
std::string_view GenericLexer::consumeLine()
{
	size_t start = m_index;
	while (!isEof() && peek() != '\r' && peek() != '\n')
		m_index++;
	size_t length = m_index - start;

	consumeSpecific('\r');
	consumeSpecific('\n');

	if (length == 0)
		return {};
	return m_input.substr(start, length);
}

// Consume and return characters while `condition` returns true
std::string_view GenericLexer::consumeWhile(Condition condition)
{
	size_t start = m_index;
	while (!isEof() && condition(peek()))
		m_index++;
	size_t length = m_index - start;

	if (length == 0)
		return {};
	return m_input.substr(start, length);
}

// Consume and return characters until `stop` is peek'd
// The `stop` character is ignored, as it is user-defined
std::string_view GenericLexer::consumeUntil(char stop)
{
	size_t start = m_index;
	while (!isEof() && peek() != stop)
		m_index++;
	size_t length = m_index - start;

	ignore();

	if (length == 0)
		return {};
	return m_input.substr(start, length);
}

// Consume and return characters until the string `stop` is found
// The `stop` string is ignored, as it is user-defined
std::string_view GenericLexer::consumeUntil(std::string_view stop)
{
	size_t start = m_index;
	while (!isEof() && !nextIs(stop))
		m_index++;
	size_t length = m_index - start;

	ignore(stop.length());

	if (length == 0)
		return {};
	return m_input.substr(start, length);
}

// Consume and return characters until `condition` return true
std::string_view GenericLexer::consumeUntil(Condition condition)
{
	size_t start = m_index;
	while (!isEof() && !condition(peek()))
		m_index++;
	size_t length = m_index - start;

	if (length == 0)
		return {};
	return m_input.substr(start, length);
}

// Consume a string surrounded by single or double quotes
// The returned string view does not include the quotes
std::optional<std::string_view> GenericLexer::consumeQuotedString()
{
	if (!nextIs('\'') && !nextIs('"'))
		return {};

	char quoteChar = consume();
	size_t start = m_index;
	while (!isEof() && !nextIs(quoteChar))
		m_index++;
	size_t length = m_index - start;

	if (peek() != quoteChar) {
		m_index = start - 1; // Restore the index in case the string is unterminated
		return {};
	}

	ignore();

	return m_input.substr(start, length);
}

// Ignore a number of characters (1 by default)
void GenericLexer::ignore(size_t count)
{
	count = std::min(count, m_input.length() - m_index);
	m_index += count;
}

// Ignore characters while `condition` returns true
void GenericLexer::ignoreWhile(Condition condition)
{
	while (!isEof() && condition(peek()))
		m_index++;
}

// Ignore characters until `stop` is peek'd
// The `stop` character is ignored as it is user-defined
void GenericLexer::ignoreUntil(char stop)
{
	while (!isEof() && !nextIs(stop))
		m_index++;
	ignore();
}

// Ignore characters until the string `stop` is found
// The `stop` string is ignored, as it is user-defined
void GenericLexer::ignoreUntil(std::string_view stop)
{
	while (!isEof() && !nextIs(stop))
		m_index++;
	ignore(stop.length());
}

// Ignore characters until `condition` return true
// We don't skip the stop character as it may not be a single value
void GenericLexer::ignoreUntil(Condition condition)
{
	while (!isEof() && !condition(peek()))
		m_index++;
}