/*
** G13, 2019
** Exception.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

namespace G13 {
	class Exception;
}

////////////////////////////////////////////////////////////////////////////////

#include <exception>
#include <string>

#include "extern/tinyformat.hpp"

////////////////////////////////////////////////////////////////////////////////

namespace G13
{

class Exception : public std::exception
{
public:
	template <typename... Args>
	Exception(const char *fmt, const Args&... args) {
		m_what = tfm::format(fmt, args...);
	}

	virtual const char *what() const noexcept override {
		return m_what.c_str();
	}

private:
	std::string m_what;
};

}