/*
** G13, 2020
** Profile.cpp
*/

#include "Profile.hpp"
#include "Common/Logger.hpp"
#include "Common/Hardware.hpp"

////////////////////////////////////////////////////////////////////////////////

namespace G13
{

////////////////////////////////////////////////////////////////////////////////

Profile::Profile(Device& parent, const std::string& name)
: m_parentDevice(parent)
, m_name(name)
, m_pages({
	{ Color(100, 0, 0), {
		{ Key::G22, std::make_shared<KeyBind>(Keyboard::Key::Space) },
		{ Key::Left, std::make_shared<ButtonBind>(Mouse::Button::Left) },
		{ Key::Down, std::make_shared<ButtonBind>(Mouse::Button::Right) },
	} },
	{ Color(0, 100, 0), {} },
	{ Color(0, 0, 100), {} }
  })
{
	switchToPage(0);
}

////////////////////////////////////////////////////////////////////////////////

void Profile::switchToPage(size_t page)
{
	assert(page < 3);

	if (page == m_currentPageIndex)
		return;

	if (m_currentPageIndex < 3)
		m_parentDevice.turnLedOff(Device::LED(1 << m_currentPageIndex));
	m_currentPageIndex = page;
	m_parentDevice.turnLedOn(Device::LED(1 << m_currentPageIndex));
	m_parentDevice.setBacklightColor(m_pages[m_currentPageIndex].color);
}

void Profile::doAction(Key key, bool pressed)
{
	auto &actions = m_pages[m_currentPageIndex].actions;
	auto it = actions.find(key);
	if (it != actions.end())
		it->second->execute(pressed);
}

////////////////////////////////////////////////////////////////////////////////

}