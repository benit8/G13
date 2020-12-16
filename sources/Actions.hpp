/*
** G13, 2020
** Actions.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

namespace G13 {
	class Action;
	class KeyBind;
	class KeySequence;
}

////////////////////////////////////////////////////////////////////////////////

#include "Common/Logger.hpp"
#include "WM/WM.hpp"

////////////////////////////////////////////////////////////////////////////////

namespace G13
{

////////////////////////////////////////////////////////////

class Action
{
public:
	virtual void execute(bool pressed) = 0;
	virtual void dump(unsigned indent) const = 0;
};

////////////////////////////////////////////////////////////

class KeyBind final : public Action
{
public:
	KeyBind(Keyboard::Key key, Keyboard::Modifier mods = Keyboard::Modifier::NoMod)
	: m_key(key)
	, m_modifiers(mods)
	{}

	void execute(bool pressed) override
	{
		WM::the().sendKeyEvent(m_key, m_modifiers, pressed);
		Logger::debug("Keybind: sent keycode %u %s", (unsigned)m_key, pressed ? "down" : "up");
	}

	void dump(unsigned) const override
	{}

private:
	Keyboard::Key m_key;
	Keyboard::Modifier m_modifiers;
};

////////////////////////////////////////////////////////////

class ButtonBind final : public Action
{
public:
	ButtonBind(Mouse::Button button)
	: m_button(button)
	{}

	void execute(bool pressed) override
	{
		WM::the().sendButtonEvent(m_button, pressed);
		Logger::debug("ButtonBind: sent button %u %s", (unsigned)m_button, pressed ? "down" : "up");
	}

	void dump(unsigned) const override
	{}

private:
	Mouse::Button m_button;
};

////////////////////////////////////////////////////////////

// class KeySequence
// {
// public:
// 	KeySequence(const std::vector<Keyboard::Key> &keys);

// };

////////////////////////////////////////////////////////////

}