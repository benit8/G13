/*
** G13, 2020
** Common / RenderTarget.cpp
*/

#include "Common/RenderTarget.hpp"
#include "Common/Exception.hpp"
#include "Common/Logger.hpp"

#include "font-5x8.h"

#include <cstring>

////////////////////////////////////////////////////////////////////////////////

RenderTarget::RenderTarget(unsigned width, unsigned height)
: m_width(width)
, m_height(height)
, m_bufferSize(width / 8 * height)
, m_framebuffer(new unsigned char[m_bufferSize])
{
	if (m_framebuffer == nullptr)
		throw Exception("RenderTarget: failed to allocate framebuffer");
}

RenderTarget::~RenderTarget()
{
	if (m_framebuffer != nullptr)
		delete[] m_framebuffer;
}

////////////////////////////////////////////////////////////////////////////////

void RenderTarget::clear()
{
	::memset(m_framebuffer, 0, m_bufferSize);
}

void RenderTarget::setPixel(unsigned x, unsigned y)
{
	unsigned offset = x + (y / 8) * m_width;
	unsigned char mask = 1 << (y % 8);

	if (offset >= m_bufferSize) {
		Logger::warn("RenderTarget::setPixel: bad offset %u for [%u,%u]", offset, x, y);
		return;
	}

	m_framebuffer[offset] |= mask;
}

void RenderTarget::writeCharacter(unsigned x, unsigned y, int c)
{
	if (c < 0 || c > 255)
		return;

	const unsigned char* lines = &console_font_5x8[c * 8];
	for (uint8_t line = 0; line < 8; ++line) {
		if (lines[line] == 0)
			continue;

		for (uint8_t _x = 0; _x < 5; ++_x) {
			if (lines[line] & (1U << _x))
				setPixel(x + _x, y + line);
		}
	}
}

void RenderTarget::writeString(unsigned x, unsigned y, std::string_view str)
{
	for (auto c : str) {
		writeCharacter(x, y, c);
		x += 5;
	}
}