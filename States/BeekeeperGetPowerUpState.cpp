#include <deque>
#include "BeekeeperGetPowerUpState.h"
#include "../Entities/Beekeeper.h"
#include "../Domain/Graph.h"
//#include "../../Powerup/Powerup.hpp"

BeekeeperGetPowerUpState::BeekeeperGetPowerUpState(Beekeeper* beekeeper)
	: IBeekeeperState(beekeeper)
{
	this->_set_path_to_powerup();
}

void BeekeeperGetPowerUpState::update(float delta_time)
{
	if (this->_beekeeper->position == this->_beekeeper->target_position) {
		this->_beekeeper->_arrive_at_target(this->_beekeeper->target_vertex);

		// Target the next thingy in the path
		if (this->_beekeeper->_path.size() > 0) {
			this->_beekeeper->_target_next_vert_in_path();
		}
		else {
			// Hey, the path is empty and we've arrived at the target vector!
			this->_arrived_at_powerup();
			//this->_beekeeper->set_state(new BeeKeeperSuperState(this->_beekeeper));
		}
	}
	else {
		this->_beekeeper->_step_towards_target(delta_time);
	}
}

void BeekeeperGetPowerUpState::_set_path_to_powerup()
{
	//vector<Vertex*> proposed_path = Pathfinding::astar(this->_beekeeper->field.field, this->_beekeeper->current_vertex, this->_beekeeper->field.powerup->current_position);
	//this->_beekeeper->_path = std::deque<Vertex*>(proposed_path.begin(), proposed_path.end());
}

void BeekeeperGetPowerUpState::_arrived_at_powerup()
{
	//this->_beekeeper->field.powerup->jump_to_random_vector();
	// According to specs, we have to empty net upon powerup
	this->_beekeeper->catchingarea.clear();
}