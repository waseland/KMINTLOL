#pragma once

#include "IBeekeeperState.h"

class BeekeeperReturnHomeState : public IBeekeeperState {
public:
	BeekeeperReturnHomeState(Beekeeper* beekeeper);

	virtual void update(float delta_time) override;
private:

	void _set_path_to_home();
	void _returned_home();
};

