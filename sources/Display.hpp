/*
** G13, 2019
** Display.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

namespace G13 {
	class Display;
}

////////////////////////////////////////////////////////////////////////////////

#include <libusb-1.0/libusb.h>

////////////////////////////////////////////////////////////////////////////////

namespace G13
{

class Display
{
public:
	static constexpr unsigned Width = 160;
	static constexpr unsigned Height = 48;

public:
	Display(libusb_device_handle *handle);
	~Display();

private:
	libusb_device_handle *m_handle = nullptr;
};

}