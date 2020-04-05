/*
** G13, 2019
** Device.cpp
*/

#include "Device.hpp"
#include "Exception.hpp"
#include "Logger.hpp"

#include <cassert>

////////////////////////////////////////////////////////////////////////////////

namespace G13
{

////////////////////////////////////////////////////////////////////////////////

size_t Device::s_instanceCount = 0;

////////////////////////////////////////////////////////////////////////////////

Device::Device(libusb_device *device)
: m_id(++s_instanceCount)
, m_handle(nullptr)
{
	int ret = libusb_open(device, &m_handle);
	if (ret < 0)
		throw G13::Exception("Error opening G13 (%s: %s)", libusb_error_name(ret), libusb_strerror((libusb_error)ret));

	// If there's a kernel driver attached to the device, we cannot take control of it
	if (libusb_kernel_driver_active(m_handle, 0) == 1) {
		Logger::debug("Device #%zu: Kernel driver is attached", m_id);

		// Try to detach the kernel driver
		if (libusb_detach_kernel_driver(m_handle, 0) == 0)
			Logger::debug("Device #%zu: Kernel driver detached", m_id);
		else
			throw G13::Exception("Could not detach kernel driver");
	}

	// Claim the interface to perform I/O on the device
	if (libusb_claim_interface(m_handle, 0) < 0)
		throw G13::Exception("Could not claim interface");

	Logger::trace("Device() #%zu: %p", m_id, this);
}

Device::~Device()
{
	libusb_release_interface(m_handle, 0);
	libusb_close(m_handle);

	Logger::trace("~Device() #%zu", m_id);
	s_instanceCount--;
}

////////////////////////////////////////////////////////////////////////////////

void Device::init()
{
	try {
		m_display = std::make_shared<LCDDisplay>(m_handle);
	}
	catch (G13::Exception &e) {
		Logger::error("Device #%zu: Could not init LCD display: ", e.what());
	}

	// Push a default profile
	// m_profiles.push_back(std::make_shared<Profile>("Default"));
}

////////////////////////////////////////////////////////////////////////////////

}