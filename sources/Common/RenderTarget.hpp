/*
** G13, 2020
** Common / RenderTarget.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

#include <cstddef>
#include <string_view>

////////////////////////////////////////////////////////////////////////////////

class RenderTarget
{
protected:
	RenderTarget(unsigned width, unsigned height);

public:
	~RenderTarget();

	void clear();
	void setPixel(unsigned x, unsigned y);
	void writeCharacter(unsigned x, unsigned y, int c);
	void writeString(unsigned x, unsigned y, std::string_view);

	virtual void present() const = 0;

	const unsigned char* getFramebufferData() const { return m_framebuffer; }
	size_t getFramebufferSize() const { return m_bufferSize; }

private:
	unsigned m_width, m_height;
	unsigned m_bufferSize;
	unsigned char* m_framebuffer = nullptr;
};