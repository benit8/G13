/*
** G13, 2019
** Manager.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

namespace G13 {
	class Manager;
}

////////////////////////////////////////////////////////////////////////////////

#include <fcntl.h>
#include <linux/uinput.h>
#include <memory>
#include <list>
#include <sys/stat.h>
#include <unistd.h>
#include <libusb-1.0/libusb.h>

#include "Device.hpp"

////////////////////////////////////////////////////////////////////////////////

namespace G13
{

constexpr uint16_t VendorId  = 0x046d; // 1133
constexpr uint16_t ProductId = 0xc21c; // 49692

class Manager
{
public:
	Manager();
	~Manager();

	int run();
	int isRunning() const { return s_running; }

	static Manager &the();

private:
	bool findDevices();

	static void sighandler(int) { s_running = false; }

private:
	std::list<std::unique_ptr<Device>> m_devices;

	static bool s_running;
};

}