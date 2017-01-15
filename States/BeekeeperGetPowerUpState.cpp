#include <deque>
#include "BeekeeperGetPowerUpState.h"
#include "../Entities/Beekeeper.h"
#include "../Domain/Graph.h"
#include "../Entities/BeekeeperPowerUp.h"

BeekeeperGetPowerUpState::BeekeeperGetPowerUpState(Beekeeper* beekeeper)
	: IBeekeeperState(beekeeper)
{
	std::cout << "Getting power up" << std::endl;
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
			this->_beekeeper->set_state(new BeekeeperPowerUpState(this->_beekeeper));
		}
	}
	else {
		this->_beekeeper->_step_towards_target(delta_time);
	}
}

void BeekeeperGetPowerUpState::_set_path_to_powerup()
{
	this->_beekeeper->_graph->target = this->_beekeeper->_graph->beekeeper_power_up->position;
	vector<Vertex*> proposed_path = Graph::find_path(*this->_beekeeper->_graph, this->_beekeeper->current_vertex, this->_beekeeper->_graph->beekeeper_power_up->position);
	this->_beekeeper->_path = std::deque<Vertex*>(proposed_path.begin(), proposed_path.end());
}

void BeekeeperGetPowerUpState::_arrived_at_powerup()
{
	this->_beekeeper->_graph->beekeeper_power_up->Move();
	for (Bee* bee : this->_beekeeper->catchingarea.caught_bees) {
		bee->SetActive(false);
		this->_beekeeper->_graph->bees_out_of_game.push_back(bee);
	}
	this->_beekeeper->catchingarea.caught_bees.clear();
}