#pragma once

#include "IBeekeeperState.h"

class BeekeeperGetPowerUpState : public IBeekeeperState {
public:
	BeekeeperGetPowerUpState(Beekeeper* beekeeper);

	virtual void update(float delta_time) override;

private:

	void _set_path_to_powerup();
	void _arrived_at_powerup();
};

