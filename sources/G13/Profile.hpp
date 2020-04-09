/*
** G13, 2020
** Joystick.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

namespace G13 {
	class Profile;
}

////////////////////////////////////////////////////////////////////////////////

#include "Color.hpp"
#include "G13/Actions.hpp"
#include "G13/Device.hpp"
#include "G13/Keys.hpp"

#include <map>
#include <memory>
#include <string>

////////////////////////////////////////////////////////////////////////////////

namespace G13
{

class Profile
{
	struct Page
	{
		Color color;
		std::map<Key, std::shared_ptr<Action>> actions;
	};

public:
	explicit Profile(Device &parent, std::string &&name = "Default");

	void switchToPage(size_t page);
	void doAction(Key) const;

	const std::string &name() const { return m_name; }

private:
	Device &m_parent;
	std::string m_name;
	Page m_pages[3];
	size_t m_currentPageIndex = 0;
};

}