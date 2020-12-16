/*
** G13, 2020
** WM / WM.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

#include "Common/Hardware.hpp"

#include <memory>

////////////////////////////////////////////////////////////////////////////////

class WM
{
public:
	static WM& the();

protected:
	WM() = default;

public:
	virtual ~WM() = default;
	virtual void dumpKeycodes() = 0;
	virtual void sendButtonEvent(Mouse::Button, bool pressed) = 0;
	virtual void sendKeyEvent(Keyboard::Key, Keyboard::Modifier, bool pressed) = 0;
};