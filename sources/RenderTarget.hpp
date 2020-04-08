/*
** G13, 2020
** RenderTarget.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

#include <cstddef>
#include <string>

////////////////////////////////////////////////////////////////////////////////

class RenderTarget
{
protected:
	RenderTarget(unsigned width, unsigned height);

public:
	~RenderTarget();

	void clear();
	void setPixel(unsigned x, unsigned y);
	void writeString(unsigned x, unsigned y, std::string &&);

	virtual void present() const = 0;

	const unsigned char *getFramebufferData() const { return m_framebuffer; }
	size_t getFramebufferSize() const { return m_bufferSize; }

private:
	unsigned m_width, m_height;
	unsigned m_bufferSize;
	unsigned char *m_framebuffer = nullptr;
};