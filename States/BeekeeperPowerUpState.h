#pragma once

#include "IBeekeeperState.h"

class Vertex;

class BeekeeperPowerUpState : public IBeekeeperState
{
public:
	BeekeeperPowerUpState(Beekeeper* beekeeper);
	~BeekeeperPowerUpState();

	virtual void update(float delta_time) override;

private:
	const int _max_bees_in_net = 30;

	void _set_next_state();

	void _recalculate_path();
	Vertex* _get_target_vertex();
};

