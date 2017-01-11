#pragma once

#include "IBeekeeperState.h"

class BeekeeperIdleState : public IBeekeeperState {
public:
	BeekeeperIdleState(Beekeeper* beekeeper);

	virtual void update(float delta_time) override;

private:
	int _last_bee_lost_timestamp;
	const int _lost_bee_interval = 1000;

	void _lose_bee();
};

