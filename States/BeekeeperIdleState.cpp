#include "BeekeeperIdleState.h"
#include <iostream>
#include "../Entities/Beekeeper.h"

BeekeeperIdleState::BeekeeperIdleState(Beekeeper* beekeeper)
	: IBeekeeperState(beekeeper)
{
	this->name = "Beekeeper idling";

	this->_beekeeper->_target_random_neighbour();
}

void BeekeeperIdleState::update(float delta_time)
{
	if (this->_beekeeper->position == this->_beekeeper->target_position) {
		this->_beekeeper->_arrive_at_target(this->_beekeeper->target_vertex);

		if (this->_beekeeper->catchingarea.caught_bees.size() == 0) {
			this->_beekeeper->set_state(new BeekeeperReturnHomeState(this->_beekeeper));
			return;
		}

		this->_beekeeper->_target_random_neighbour();
	}
	else {
		this->_beekeeper->_step_towards_target(delta_time);
	}

	int now = this->_beekeeper->mApplication->GetTimeSinceStartedMS();
	if (now > this->_last_bee_lost_timestamp + this->_lost_bee_interval) {
		this->_lose_bee();
	}
}

void BeekeeperIdleState::_lose_bee()
{
	this->_last_bee_lost_timestamp = this->_beekeeper->mApplication->GetTimeSinceStartedMS();
	this->_beekeeper->catchingarea.lose_bee();
}