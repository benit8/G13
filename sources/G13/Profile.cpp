/*
** G13, 2020
** Profile.cpp
*/

#include "G13/Profile.hpp"
#include "Logger.hpp"

////////////////////////////////////////////////////////////////////////////////

namespace G13
{

////////////////////////////////////////////////////////////////////////////////

Profile::Profile(Device &parent, std::string &&name)
: m_parent(parent)
, m_name(name)
, m_pages({
	{ Color(100, 0, 0), {} },
	{ Color(0, 100, 0), {} },
	{ Color(0, 0, 100), {} }
  })
{
	switchToPage(0);
}

////////////////////////////////////////////////////////////////////////////////

void Profile::switchToPage(size_t page)
{
	if (page >= 3) {
		Logger::warn("Tried switching to page #%zu in profile %s", page, m_name);
		return;
	}
	else if (page == m_currentPageIndex) {
		return;
	}

	if (m_currentPageIndex < 3)
		m_parent.turnLedOff(Device::LED(1 << m_currentPageIndex));
	m_currentPageIndex = page;
	m_parent.turnLedOn(Device::LED(1 << m_currentPageIndex));
	m_parent.setBacklightColor(m_pages[m_currentPageIndex].color);
}

void Profile::doAction(Key) const
{}

////////////////////////////////////////////////////////////////////////////////

}