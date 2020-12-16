/*
** G13, 2020
** Common / Hardware.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

class Keyboard
{
public:
	enum class Key
	{
		Backspace,
		Delete,
		Escape,
		Return,
		Space,
	};

	enum class Modifier
	{
		NoMod,
		Control,
		Shift,
		Alt,
		AltGr,
		CapsLock,
		NumLock,
	};
};

////////////////////////////////////////////////////////////////////////////////

class Mouse
{
public:
	enum class Button
	{
		Left,
		Middle,
		Right,
		ScrollUp,
		ScrollDown,
		ScrollLeft,
		ScrollRight,
		Previous,
		Next,
	};
};