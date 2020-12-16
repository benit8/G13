/*
** G13, 2020
** Common / Color.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

struct Color
{
public:
	Color()
	: r(0)
	, g(0)
	, b(0)
	{}

	Color(unsigned char _r, unsigned char _g, unsigned char _b)
	: r(_r)
	, g(_g)
	, b(_b)
	{}

public:
	unsigned char r = 0, g = 0, b = 0;
};