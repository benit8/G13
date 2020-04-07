/*
** G13, 2019
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


	{
		unsigned char data[5] = { 5, 0, 0, 0, 0 };
		data[1] = 0/*leds*/;

		int ret = libusb_control_transfer(m_handle, LIBUSB_REQUEST_TYPE_CLASS | LIBUSB_RECIPIENT_INTERFACE, 9, 0x305, 0, data, sizeof(data), 1000);
		if (ret != sizeof(data)) {
			Logger::error("Problem sending leds data");
		}
	}

	{
		unsigned char data[5] = { 5, 0, 0, 0, 0 };
		data[1] = 100/*red*/;
		data[2] = 0/*green*/;
		data[3] = 0/*blue*/;

		int ret = libusb_control_transfer(m_handle, LIBUSB_REQUEST_TYPE_CLASS | LIBUSB_RECIPIENT_INTERFACE, 9, 0x307, 0, data, sizeof(data), 1000);
		if (ret != sizeof(data)) {
			Logger::error("Problem sending key color data");
		}
	}
}

LCDDisplay::~LCDDisplay()
{
}

////////////////////////////////////////////////////////////////////////////////

}