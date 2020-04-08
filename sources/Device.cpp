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

const std::map<Device::Key, Device::KeyInfo> Device::s_keys = {
	{Device::Key::G1, {"G1", 3, 1}},
	{Device::Key::G2, {"G2", 3, 2}},
	{Device::Key::G3, {"G3", 3, 4}},
	{Device::Key::G4, {"G4", 3, 8}},
	{Device::Key::G5, {"G5", 3, 16}},
	{Device::Key::G6, {"G6", 3, 32}},
	{Device::Key::G7, {"G7", 3, 64}},
	{Device::Key::G8, {"G8", 3, 128}},
	{Device::Key::G9, {"G9", 4, 1}},
	{Device::Key::G10, {"G10", 4, 2}},
	{Device::Key::G11, {"G11", 4, 4}},
	{Device::Key::G12, {"G12", 4, 8}},
	{Device::Key::G13, {"G13", 4, 16}},
	{Device::Key::G14, {"G14", 4, 32}},
	{Device::Key::G15, {"G15", 4, 64}},
	{Device::Key::G16, {"G16", 4, 128}},
	{Device::Key::G17, {"G17", 5, 1}},
	{Device::Key::G18, {"G18", 5, 2}},
	{Device::Key::G19, {"G19", 5, 4}},
	{Device::Key::G20, {"G20", 5, 8}},
	{Device::Key::G21, {"G21", 5, 16}},
	{Device::Key::G22, {"G22", 5, 32}},
	{Device::Key::_Undef1, {"_Undef1", 5, 64}},
	{Device::Key::LightState, {"LightState", 5, 128}},
	{Device::Key::BD, {"BD", 6, 1}},
	{Device::Key::L1, {"L1", 6, 2}},
	{Device::Key::L2, {"L2", 6, 4}},
	{Device::Key::L3, {"L3", 6, 8}},
	{Device::Key::L4, {"L4", 6, 16}},
	{Device::Key::M1, {"M1", 6, 32}},
	{Device::Key::M2, {"M2", 6, 64}},
	{Device::Key::M3, {"M3", 6, 128}},
	{Device::Key::MR, {"MR", 7, 1}},
	{Device::Key::Left, {"Left", 7, 2}},
	{Device::Key::Down, {"Down", 7, 4}},
	{Device::Key::Top, {"Top", 7, 8}},
	{Device::Key::_Undef2, {"_Undef2", 7, 16}},
	{Device::Key::Light, {"Light", 7, 32}},
	{Device::Key::Light2, {"Light2", 7, 64}},
	{Device::Key::MiscToggle, {"MiscToggle", 7, 128}},
};

////////////////////////////////////////////////////////////////////////////////

Device::Device(libusb_device *device)
: m_id(++s_instanceCount)
, m_handle(claimDevice(device))
, m_display(m_handle)
{
	Logger::trace("Device() #%zu: %p", m_id, this);

	m_profiles.emplace_back("Default");
}

Device::~Device()
{
	libusb_release_interface(m_handle, 0);
	libusb_close(m_handle);

	Logger::trace("~Device() #%zu", m_id);
	s_instanceCount--;
}

////////////////////////////////////////////////////////////////////////////////

/// IDEA: https://stackoverflow.com/questions/1262310/simulate-keypress-in-a-linux-c-console-application
/// IDEA: xdotool
void Device::readKeys()
{
	int transfered = 0;
	int error = libusb_interrupt_transfer(m_handle, LIBUSB_ENDPOINT_IN | KeyEndpoint, m_keyBuffer, ReportSize, &transfered, 100);

	if (error != 0 && error != LIBUSB_ERROR_TIMEOUT) {
		Logger::error("Device #%zu: Error while reading keys: %s (%s)", m_id, libusb_error_name(error), libusb_strerror((libusb_error)error));
		return;
	}
	else if (error == LIBUSB_ERROR_TIMEOUT) {
		// LIBUSB_ERROR_TIMEOUT happens everytime you don't press anything, so
		// it's not really an error. But we don't want to go any further.
		return;
	}

	if (transfered != ReportSize) {
		Logger::error("Device #%zu: Wrong byte count transfered: got %d, expected %u", m_id, transfered, ReportSize);
		return;
	}

	// Logger::debug("Device #%zu: key buffer {%u, %u, %u, %u, %u, %u, %u, %u}", m_id, m_keyBuffer[0], m_keyBuffer[1], m_keyBuffer[2], m_keyBuffer[3], m_keyBuffer[4], m_keyBuffer[5], m_keyBuffer[6], m_keyBuffer[7]);
	parseJoystick();
	parseKeys();

	// send_event(EV_SYN, SYN_REPORT, 0);
}

////////////////////////////////////////////////////////////////////////////////

libusb_device_handle *Device::claimDevice(libusb_device *device)
{
	libusb_device_handle *handle = nullptr;
	int ret = libusb_open(device, &handle);
	if (ret < 0)
		throw G13::Exception("Error opening G13: %s (%s)", libusb_error_name(ret), libusb_strerror((libusb_error)ret));

	// If there's a kernel driver attached to the device, we cannot take control of it
	if (libusb_kernel_driver_active(handle, 0) == 1) {
		Logger::debug("Device #%zu: Kernel driver is attached", m_id);

		// Try to detach the kernel driver
		if (libusb_detach_kernel_driver(handle, 0) == 0)
			Logger::debug("Device #%zu: Kernel driver detached", m_id);
		else
			throw G13::Exception("Could not detach kernel driver");
	}

	// Claim the interface to perform I/O on the device
	if (libusb_claim_interface(handle, 0) < 0)
		throw G13::Exception("Could not claim interface");

	return handle;
}

void Device::parseJoystick()
{
	float x = (m_keyBuffer[1] - 127.5f) / 127.5f;
	float y = (m_keyBuffer[2] - 127.5f) / 127.5f;

	Logger::debug("Device #%zu: joystick at %.2f %.2f", m_id, x, y);
}

void Device::parseKeys()
{
	for (const auto &[key, infos] : s_keys) {
		bool pressed = m_keyBuffer[infos.byte] & infos.bit;
		if (m_keyStates[key] != pressed)
			Logger::debug("Device #%zu: key %s %s", m_id, infos.name, pressed ? "pressed" : "released");
		m_keyStates[key] = pressed;
	}
}

////////////////////////////////////////////////////////////////////////////////

}