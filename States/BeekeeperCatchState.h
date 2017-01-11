#pragma once

#include "IBeekeeperState.h"

class Vertex;

class BeekeeperCatchState : public IBeekeeperState {
public:
	BeekeeperCatchState(Beekeeper* beekeeper);

	virtual void update(float delta_time) override;

private:

	const int _max_bees_in_net = 10;

	void _set_next_state();

	void _recalculate_path();
	Vertex* _get_target_vertex();
};

