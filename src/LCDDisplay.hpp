/*
** EPITECH PROJECT, 2019
** G&"
** File description:
** LCDDisplay.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

namespace G13 {
	class LCDDisplay;
}

////////////////////////////////////////////////////////////////////////////////

#include <libusb-1.0/libusb.h>

////////////////////////////////////////////////////////////////////////////////

namespace G13
{

class LCDDisplay
{
public:
	const unsigned width = 160;
	const unsigned height = 48;

public:
	LCDDisplay(libusb_device_handle *handle);
	~LCDDisplay();

private:
	libusb_device_handle *m_handle = nullptr;
};

}