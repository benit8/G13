/*
** G13, 2019
** Device.cpp
*/

#include "Device.hpp"
#include "Common/Exception.hpp"
#include "Common/Logger.hpp"

#include <cassert>

////////////////////////////////////////////////////////////////////////////////

namespace G13
{

////////////////////////////////////////////////////////////////////////////////

Device::Device(libusb_device* device)
: m_id(++s_instanceCount)
, m_handle(claimDevice(device))
, m_display(m_handle)
{
	Logger::trace("Device() #%zu: %p", m_id, this);

	m_profiles.emplace_back(*this, "Default");
	m_currentProfile = &m_profiles.back();

	readKeys();

	m_display.clear();
	m_display.writeString(0, 0, "Linked to g13d!");
	m_display.present();
}

Device::~Device()
{
	libusb_release_interface(m_handle, 0);
	libusb_close(m_handle);

	Logger::trace("~Device() #%zu", m_id);
	s_instanceCount--;
}

////////////////////////////////////////////////////////////////////////////////

void Device::readKeys()
{
	int transfered = 0;
	int error = libusb_interrupt_transfer(m_handle, LIBUSB_ENDPOINT_IN | keyEndpoint, m_keyBuffer, reportSize, &transfered, 100);

	if (error != 0 && error != LIBUSB_ERROR_TIMEOUT) {
		Logger::error("Device #%zu: Error while reading keys: %s", m_id, libusb_strerror((libusb_error)error));
		return;
	}
	else if (error == LIBUSB_ERROR_TIMEOUT) {
		// LIBUSB_ERROR_TIMEOUT happens everytime you don't press anything, so
		// it's not really an error. But we don't want to go any further.
		return;
	}

	if (transfered != reportSize) {
		Logger::error("Device #%zu: Wrong byte count transfered: got %d, expected %u", m_id, transfered, reportSize);
		return;
	}

	parseJoystick();
	parseKeys();
}

void Device::setBacklightColor(const Color& c)
{
	unsigned char data[5] = { 5, c.r, c.g, c.b, 0 };
	int ret = libusb_control_transfer(m_handle, LIBUSB_REQUEST_TYPE_CLASS | LIBUSB_RECIPIENT_INTERFACE, 9, 0x307, 0, data, sizeof(data), 1000);
	if (ret != sizeof(data)) {
		Logger::error("Device #%zu: Problem sending key color data", m_id);
	}
}

////////////////////////////////////////////////////////////////////////////////

libusb_device_handle* Device::claimDevice(libusb_device* device)
{
	libusb_device_handle *handle = nullptr;
	int ret = libusb_open(device, &handle);
	if (ret < 0)
		throw Exception("Error opening G13: %s", libusb_strerror((libusb_error)ret));

	// If there's a kernel driver attached to the device, we cannot take control of it
	if (libusb_kernel_driver_active(handle, 0) == 1) {
		Logger::debug("Device #%zu: Kernel driver is attached", m_id);

		// Try to detach the kernel driver
		if (libusb_detach_kernel_driver(handle, 0) == 0)
			Logger::debug("Device #%zu: Kernel driver detached", m_id);
		else
			throw Exception("Could not detach kernel driver");
	}

	// Claim the interface to perform I/O on the device
	if (libusb_claim_interface(handle, 0) < 0)
		throw Exception("Could not claim interface");

	return handle;
}

void Device::transferLedState()
{
	unsigned char data[5] = { 5, m_ledState, 0, 0, 0 };

	int ret = libusb_control_transfer(m_handle, LIBUSB_REQUEST_TYPE_CLASS | LIBUSB_RECIPIENT_INTERFACE, 9, 0x305, 0, data, sizeof(data), 1000);
	if (ret != sizeof(data)) {
		Logger::error("Device #%zu: Problem sending leds data", m_id);
	}
}

void Device::parseJoystick()
{
	float x = (m_keyBuffer[1] - 127.5f) / 127.5f;
	float y = (m_keyBuffer[2] - 127.5f) / 127.5f;

	// Logger::debug("Device #%zu: joystick at %.2f %.2f", m_id, x, y);
}

void Device::parseKeys()
{
	for (const auto &[key, infos] : s_keys) {
		bool newState = m_keyBuffer[infos.byte] & infos.bit;
		if (m_keyStates[key] == newState)
			continue;

		m_keyStates[key] = newState;
		// Logger::debug("Device #%zu: key %s %s", m_id, infos.name, newState ? "pressed" : "released");

		if (!infos.bindable)
			doBuiltinAction(key, newState);
		else
			m_currentProfile->doAction(key, newState);
	}
}

void Device::doBuiltinAction(Key key, bool pressed)
{
	if (!pressed)
		return;

	switch (key) {
		case Key::M1:
			m_currentProfile->switchToPage(0);
			break;
		case Key::M2:
			m_currentProfile->switchToPage(1);
			break;
		case Key::M3:
			m_currentProfile->switchToPage(2);
			break;
		default:
			break;
	}
}

////////////////////////////////////////////////////////////////////////////////

size_t Device::s_instanceCount = 0;

const std::map<Key, Device::KeyInfo> Device::s_keys = {
	{ Key::G1,         {"G1",         3,   1, true}  },
	{ Key::G2,         {"G2",         3,   2, true}  },
	{ Key::G3,         {"G3",         3,   4, true}  },
	{ Key::G4,         {"G4",         3,   8, true}  },
	{ Key::G5,         {"G5",         3,  16, true}  },
	{ Key::G6,         {"G6",         3,  32, true}  },
	{ Key::G7,         {"G7",         3,  64, true}  },
	{ Key::G8,         {"G8",         3, 128, true}  },
	{ Key::G9,         {"G9",         4,   1, true}  },
	{ Key::G10,        {"G10",        4,   2, true}  },
	{ Key::G11,        {"G11",        4,   4, true}  },
	{ Key::G12,        {"G12",        4,   8, true}  },
	{ Key::G13,        {"G13",        4,  16, true}  },
	{ Key::G14,        {"G14",        4,  32, true}  },
	{ Key::G15,        {"G15",        4,  64, true}  },
	{ Key::G16,        {"G16",        4, 128, true}  },
	{ Key::G17,        {"G17",        5,   1, true}  },
	{ Key::G18,        {"G18",        5,   2, true}  },
	{ Key::G19,        {"G19",        5,   4, true}  },
	{ Key::G20,        {"G20",        5,   8, true}  },
	{ Key::G21,        {"G21",        5,  16, true}  },
	{ Key::G22,        {"G22",        5,  32, true}  },
	{ Key::_Undef1,    {"_Undef1",    5,  64, false} },
	{ Key::LightState, {"LightState", 5, 128, false} },
	{ Key::BD,         {"BD",         6,   1, false} },
	{ Key::L1,         {"L1",         6,   2, false} },
	{ Key::L2,         {"L2",         6,   4, false} },
	{ Key::L3,         {"L3",         6,   8, false} },
	{ Key::L4,         {"L4",         6,  16, false} },
	{ Key::M1,         {"M1",         6,  32, false} },
	{ Key::M2,         {"M2",         6,  64, false} },
	{ Key::M3,         {"M3",         6, 128, false} },
	{ Key::MR,         {"MR",         7,   1, false} },
	{ Key::Left,       {"Left",       7,   2, true}  },
	{ Key::Down,       {"Down",       7,   4, true}  },
	{ Key::Top,        {"Top",        7,   8, true}  },
	{ Key::_Undef2,    {"_Undef2",    7,  16, false} },
	{ Key::Light,      {"Light",      7,  32, false} },
	{ Key::Light2,     {"Light2",     7,  64, false} },
	{ Key::MiscToggle, {"MiscToggle", 7, 128, false} },
};

////////////////////////////////////////////////////////////////////////////////

}