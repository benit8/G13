/*
** G13, 2020
** Actions.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

namespace G13 {
	class Action;
	class KeyBind;
	class KeyCombination;
	class KeyRecord;
}

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////

namespace G13
{

////////////////////////////////////////////////////////////

class Action
{
public:
	virtual void execute() = 0;
};

////////////////////////////////////////////////////////////

class KeyBind final : public Action
{
public:
	KeyBind();

	void execute() override {}
};

////////////////////////////////////////////////////////////

}