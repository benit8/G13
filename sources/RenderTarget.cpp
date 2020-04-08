/*
** G13, 2020
** RenderTarget.cpp
*/

#include "RenderTarget.hpp"
#include "Exception.hpp"
#include "Logger.hpp"

#include "extern/font8x8/font8x8.h"

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
		Logger::warn("RenderTarget: bad offset %u for [%u,%u]", offset, x, y);
		return;
	}

	m_framebuffer[offset] |= mask;
}

void RenderTarget::writeString(unsigned x, unsigned y, std::string &&str)
{
	auto drawCharacter = [&] (int c) {
		uint64_t bitmap = font8x8_get_character(c);

		for (int _y = 0; _y < 8; ++_y)
			for (int _x = 0; _x < 8; ++_x)
				if (bitmap & (1UL << (_y * 8 + _x)))
					setPixel(_x + x, _y + y);
	};

	for (auto c : str) {
		drawCharacter(c);
		x += 8;
	}
}