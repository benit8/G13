/*
** EPITECH PROJECT, 2019
** G13
** File description:
** LCDDisplay.cpp
*/

#include "LCDDisplay.hpp"
#include "Exception.hpp"
#include "Logger.hpp"

////////////////////////////////////////////////////////////////////////////////

namespace G13
{

////////////////////////////////////////////////////////////////////////////////

LCDDisplay::LCDDisplay(libusb_device_handle *handle)
: m_handle(handle)
{
	int ret = libusb_control_transfer(m_handle, 0, 9, 1, 0, 0, 0, 1000);
	if (ret != 0) {
		Logger::error("Failed to initialize LCD display");
		throw G13::Exception("%s: %s", libusb_error_name(ret), libusb_strerror((libusb_error)ret));
	}
}

LCDDisplay::~LCDDisplay()
{
}

////////////////////////////////////////////////////////////////////////////////

}