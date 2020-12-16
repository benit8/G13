/*
** G13, 2019
** Display.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

namespace G13 {
	class Display;
}

////////////////////////////////////////////////////////////////////////////////

#include "Common/RenderTarget.hpp"

#include <libusb-1.0/libusb.h>

////////////////////////////////////////////////////////////////////////////////

namespace G13
{

/*
 * Pixels are mapped rather strangely for G13 buffer...
 *
 * Byte 0 contains column 0, row [0,7]
 * Byte 1 contains column 1, row [0,7]
 *
 * So the masks for each pixel are laid out as below (ByteOffset:PixelMask)
 *
 * 0x00:01 0x01:01 0x02:01 ...
 * 0x00:02 0x01:02 0x02:02 ...
 * 0x00:04 0x01:04 0x02:04 ...
 * 0x00:08 0x01:08 0x02:08 ...
 * 0x00:10 0x01:10 0x02:10 ...
 * 0x00:20 0x01:20 0x02:20 ...
 * 0x00:40 0x01:40 0x02:40 ...
 * 0x00:80 0x01:80 0x02:80 ...
 * 0xA0:01 0xA1:01 0xA2:01 ...
 */

class Display : public RenderTarget
{
public:
	static constexpr unsigned width = 160;
	static constexpr unsigned height = 48;

public:
	Display(libusb_device_handle* handle);

	void present() const override;

protected:
	static constexpr int usbEndPoint = 0x02;

private:
	libusb_device_handle* m_handle = nullptr;
};

}