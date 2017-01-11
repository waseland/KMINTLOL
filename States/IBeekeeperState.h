#pragma once

#include <string>
#include <algorithm>

class Beekeeper;

class IBeekeeperState
{
public:
	IBeekeeperState(Beekeeper* beekeeper);
	virtual void update(float delta_time) = 0;

	std::string name;
protected:
	Beekeeper* _beekeeper;
};

