/*
** G13, 2019
** Display.cpp
*/

#include "G13/Display.hpp"
#include "Exception.hpp"
#include "Logger.hpp"

#include <cstring>

////////////////////////////////////////////////////////////////////////////////

namespace G13
{

////////////////////////////////////////////////////////////////////////////////

Display::Display(libusb_device_handle *handle)
: RenderTarget(Width, Height)
, m_handle(handle)
{
	int ret = libusb_control_transfer(m_handle, 0, 9, 1, 0, 0, 0, 1000);
	if (ret != 0) {
		Logger::error("Failed to initialize LCD display");
		throw Exception("%s: %s", libusb_error_name(ret), libusb_strerror((libusb_error)ret));
	}


	// {
	// 	unsigned char data[5] = { 5, 0, 0, 0, 0 };
	// 	data[1] = 0/*leds*/;

	// 	int ret = libusb_control_transfer(m_handle, LIBUSB_REQUEST_TYPE_CLASS | LIBUSB_RECIPIENT_INTERFACE, 9, 0x305, 0, data, sizeof(data), 1000);
	// 	if (ret != sizeof(data)) {
	// 		Logger::error("Problem sending leds data");
	// 	}
	// }
}

////////////////////////////////////////////////////////////////////////////////

void Display::present() const
{
	size_t framebufferSize = getFramebufferSize();
	unsigned char buffer[framebufferSize + 32] = {0};
	buffer[0] = 0x03;
	::memcpy(buffer + 32, getFramebufferData(), framebufferSize);

	int transfered = 0;
	int error = libusb_interrupt_transfer(m_handle, LIBUSB_ENDPOINT_OUT | UsbEndPoint, buffer, framebufferSize + 32, &transfered, 1000);
	if (error != 0)
		Logger::error("Display: error transferring image: %s. %d/%d", libusb_strerror((libusb_error)error), transfered, framebufferSize + 32);
}

////////////////////////////////////////////////////////////////////////////////

}