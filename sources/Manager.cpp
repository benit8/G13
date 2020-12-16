/*
** G13, 2019
** Manager.cpp
*/

#include "Manager.hpp"
#include "Common/Exception.hpp"
#include "Common/Logger.hpp"

#include <cassert>
#include <csignal>
#include <cstdlib>
#include <exception>

////////////////////////////////////////////////////////////////////////////////

static G13::Manager* s_instance = nullptr;

////////////////////////////////////////////////////////////////////////////////

namespace G13
{

////////////////////////////////////////////////////////////////////////////////

bool Manager::s_running = false;

Manager& Manager::the()
{
	assert(s_instance);
	return *s_instance;
}

////////////////////////////////////////////////////////////////////////////////

Manager::Manager()
{
	Logger::trace("Manager(): %p", this);

	assert(s_instance == nullptr);
	s_instance = this;

	if (libusb_init(NULL) < 0)
		throw Exception("Could not initialize libusb");

	libusb_set_option(NULL, LIBUSB_OPTION_LOG_LEVEL, LIBUSB_LOG_LEVEL_WARNING);

	if (std::signal(SIGINT, &Manager::sighandler) == SIG_ERR)
		Logger::error("Cannot catch SIGINT");
	if (std::signal(SIGTERM, &Manager::sighandler) == SIG_ERR)
		Logger::error("Cannot catch SIGTERM");
}

Manager::~Manager()
{
	Logger::warn("Shutting down...");

	m_devices.clear();
	libusb_exit(NULL);

	Logger::trace("~Manager()");
}

////////////////////////////////////////////////////////////////////////////////

int Manager::run()
{
	Logger::info("Launching...");
	s_running = true;

	if (!findDevices()) {
		Logger::fatal("Could not find any G13");
		return EXIT_FAILURE;
	}

	Logger::info("Found %zu G13s", m_devices.size());

	while (s_running) {
		for (auto &device : m_devices) {
			device->readKeys();
		}
	}

	return EXIT_SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////

bool Manager::findDevices()
{
	// Getting usb devices complete list
	libusb_device** devices = NULL;
	ssize_t count = libusb_get_device_list(NULL, &devices);
	if (count < 0) {
		Logger::error("Could not retrieve device list");
		return false;
	}

	Logger::debug("Got %zd libusb device(s)", count);

	// Searching for G13s
	for (size_t i = 0; i < (size_t)count; ++i) {
		// Get description of device
		libusb_device_descriptor desc;
		if (libusb_get_device_descriptor(devices[i], &desc) < 0) {
			Logger::error("Failed to get device descriptor");
			continue;
		}

		// Test for a G13
		if (desc.idVendor == G13::VendorId) {
			if (desc.idProduct == G13::ProductId) {
				try {
					m_devices.push_back(std::make_unique<Device>(devices[i]));
				}
				catch (Exception& e) {
					Logger::error("Device instanciation failed: %s", e.what());
				}
			}
			else {
				Logger::info("Found other Logitech device with product ID 0x%x", desc.idProduct);
			}
		}
	}

	// Free all-devices array
	libusb_free_device_list(devices, 1);

	return m_devices.size() > 0;
}

////////////////////////////////////////////////////////////////////////////////

}