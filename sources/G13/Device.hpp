/*
** G13, 2019
** Device.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

namespace G13 {
	class Device;
}

////////////////////////////////////////////////////////////////////////////////

#include "G13/Display.hpp"
#include "G13/Keys.hpp"
#include "G13/Profile.hpp"

#include <libusb-1.0/libusb.h>
#include <list>
#include <map>
#include <memory>

////////////////////////////////////////////////////////////////////////////////

namespace G13
{

class Device
{
	static constexpr unsigned char KeyEndpoint = 1;
	static constexpr unsigned int ReportSize = 8;

	struct KeyInfo
	{
		const char *name;
		unsigned char byte;
		unsigned char bit;
	};

public:
	Device(libusb_device *device);
	~Device();

	void readKeys();
	void setBacklightColor(unsigned char r, unsigned char g, unsigned char b);

	bool isKeyPressed(Key k) const { return m_keyStates.at(k); }
	bool isDisplayBacklightOn() const { return isKeyPressed(Key::LightState); }

	size_t getId() const { return m_id; }

private:
	libusb_device_handle *claimDevice(libusb_device *device);
	void parseJoystick();
	void parseKeys();

	static size_t instanceCount() { return s_instanceCount; }

private:
	size_t m_id = 0;
	libusb_device_handle *m_handle = nullptr;

	Display m_display;

	std::list<Profile> m_profiles;
	Profile *m_currentProfile = nullptr;

	std::map<Key, bool> m_keyStates;

	/*
	 * Buffer elements description:
	 * [0]: ? (always 1?)
	 * [1]: Joystick X
	 * [2]: Joystick Y
	 * [3]: Key mask: G1 -> G8
	 * [4]: Key mask: G9 -> G16
	 * [5]: Key mask: G17 -> LightState
	 * [6]: Key mask: BD -> M3
	 * [7]: Key mask: MR -> MiscToggle
	 */
	unsigned char m_keyBuffer[ReportSize] { 0 };

	static size_t s_instanceCount;
	static const std::map<Key, KeyInfo> s_keys;
};

}