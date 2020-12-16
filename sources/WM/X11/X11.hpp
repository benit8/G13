/*
** G13, 2020
** WM / X11 / X11.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

#include "WM/WM.hpp"

#include <X11/Xlib.h>
#include <map>

////////////////////////////////////////////////////////////////////////////////

class X11 : public WM
{
public:
	X11();
	~X11();

	void dumpKeycodes() override
	{
		printf("BackSpace: %d\n", XKeysymToKeycode(m_display, XStringToKeysym("BackSpace")));
		printf("A: %d\n",         XKeysymToKeycode(m_display, XStringToKeysym("A")));
		printf("Shift_L: %d\n",   XKeysymToKeycode(m_display, XStringToKeysym("Shift_L")));
		printf("space: %d\n",     XKeysymToKeycode(m_display, XStringToKeysym("space")));
	}

	void sendButtonEvent(Mouse::Button, bool pressed) override;
	void sendKeyEvent(Keyboard::Key, Keyboard::Modifier, bool pressed) override;

private:
	Window getFocusedWindow();

private:
	Display* m_display = NULL;

	static const std::map<Mouse::Button, unsigned> s_buttonToX11Button;
};