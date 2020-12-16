/*
** G13, 2020
** Profile.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

namespace G13 {
	class Profile;
}

////////////////////////////////////////////////////////////////////////////////

#include "Common/Color.hpp"
#include "Actions.hpp"
#include "Device.hpp"
#include "Keys.hpp"

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
	explicit Profile(Device &parent, const std::string& name);

	void switchToPage(size_t page);
	void doAction(Key, bool pressed);

	const std::string& name() const { return m_name; }

private:
	Device& m_parentDevice;
	std::string m_name;

	Page m_pages[3];
	size_t m_currentPageIndex = -1;
};

}