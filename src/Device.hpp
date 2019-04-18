/*
** EPITECH PROJECT, 2019
** G13
** File description:
** Device.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

namespace G13 {
	class Device;
}

////////////////////////////////////////////////////////////////////////////////

#include <memory>
#include <list>
#include <libusb-1.0/libusb.h>

#include "LCDDisplay.hpp"
#include "Profile.hpp"

////////////////////////////////////////////////////////////////////////////////

namespace G13
{

class Device
{
public:
	Device(libusb_device *device);
	~Device();

	void init();

	size_t getId() const { return m_id; }

private:
	static size_t instanceCount() { return s_instanceCount; }

private:
	size_t m_id = 0;
	libusb_device_handle *m_handle = nullptr;
	std::shared_ptr<LCDDisplay> m_display;
	// std::list<std::shared_ptr<Profile>> m_profiles;

	static size_t s_instanceCount;
};

}