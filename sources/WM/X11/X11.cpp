/*
** G13, 2020
** WM / X11 / X11.cpp
*/

#include "WM/X11/X11.hpp"
#include "Common/Logger.hpp"
#include <cassert>

////////////////////////////////////////////////////////////////////////////////

X11::X11()
: m_display(XOpenDisplay(NULL))
{
	assert(m_display);
}

X11::~X11()
{
	XCloseDisplay(m_display);
	m_display = NULL;
}

////////////////////////////////////////////////////////////////////////////////

// https://github.com/jordansissel/xdotool/blob/master/xdo.c#L802
void X11::sendButtonEvent(Mouse::Button btn, bool pressed)
{
	XButtonEvent event;
	event.display = m_display;
	event.window = getFocusedWindow();
	event.time = CurrentTime;
	event.same_screen = XQueryPointer(m_display, event.window, &event.root, &event.subwindow, &event.x_root, &event.y_root, &event.x, &event.y, &event.state);
	event.type = pressed ? ButtonPress : ButtonRelease;
	event.button = s_buttonToX11Button.at(btn);

	XSendEvent(m_display, event.window, True, ButtonPressMask, (XEvent*)&event);
	XFlush(m_display);
}

void X11::sendKeyEvent(Keyboard::Key key, Keyboard::Modifier /*mods*/, bool pressed)
{
	XKeyEvent event;
	event.display = m_display;
	event.window = getFocusedWindow();
	event.subwindow = None;
	event.time = CurrentTime;
	event.same_screen = True;
	event.x = event.y = event.x_root = event.y_root = 1;
	event.keycode = 65; /*space*/ // Key code
	event.state = 0;              // Modifiers
	event.type = pressed ? KeyPress : KeyRelease;

	XSendEvent(m_display, event.window, True, KeyPressMask, (XEvent*)&event);
	XFlush(m_display);
}

////////////////////////////////////////////////////////////////////////////////

Window X11::getFocusedWindow()
{
	Window win = None;
	int revertTo = 0;
	XGetInputFocus(m_display, &win, &revertTo);
	return win;
}

////////////////////////////////////////////////////////////////////////////////

WM& WM::the()
{
	static WM* s_instance = nullptr;

	if (s_instance == nullptr) {
		s_instance = new X11();
	}

	return *s_instance;
}

////////////////////////////////////////////////////////////////////////////////

const std::map<Mouse::Button, unsigned> X11::s_buttonToX11Button = {
	{ Mouse::Button::Left,   Button1 },
	{ Mouse::Button::Middle, Button2 },
	{ Mouse::Button::Right,  Button3 },
};