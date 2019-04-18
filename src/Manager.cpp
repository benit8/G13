/*
** EPITECH PROJECT, 2019
** G13
** File description:
** Manager.cpp
*/

#include "Manager.hpp"
#include "Exception.hpp"
#include "Logger.hpp"

#include <cassert>
#include <csignal>
#include <cstdlib>
#include <exception>

////////////////////////////////////////////////////////////////////////////////

namespace G13
{

////////////////////////////////////////////////////////////////////////////////

static Manager *s_instance = nullptr;

bool Manager::s_running = false;


Manager &Manager::instance()
{
	assert(s_instance);
	return *s_instance;
}

////////////////////////////////////////////////////////////////////////////////

Manager::Manager()
{
	Logger::trace("+ Manager: %p", this);
	assert(s_instance == nullptr);
	s_instance = this;

	if (libusb_init(NULL) < 0)
		throw G13::Exception("Could not initialize libusb");

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

	Logger::trace("- Manager");
}

////////////////////////////////////////////////////////////////////////////////

int Manager::run()
{
	Logger::info("Launching...");

	if (!findDevices()) {
		Logger::fatal("Could not find any G13");
		return EXIT_FAILURE;
	}

	Logger::info("Found %zu G13s", m_devices.size());

	for (auto &dev : m_devices)
		dev->init();

	return EXIT_SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////

bool Manager::findDevices()
{
	// Getting usb devices complete list
	libusb_device **devices = NULL;
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
		if (desc.idVendor == G13::VendorId && desc.idProduct == G13::ProductId) {
			try {
				m_devices.push_back(std::make_shared<Device>(devices[i]));
			}
			catch (G13::Exception &e) {
				Logger::error("Device instanciation failed: %s", e.what());
			}
		}
	}

	// Free all-devices array
	libusb_free_device_list(devices, 1);

	return m_devices.size() > 0;
}

////////////////////////////////////////////////////////////////////////////////

}