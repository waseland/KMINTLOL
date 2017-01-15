#include "BeekeeperPowerUpState.h"
#include <deque>
#include <iostream>
#include "../Domain/Graph.h"
#include "../Entities/BeeKeeper.h"

BeekeeperPowerUpState::BeekeeperPowerUpState(Beekeeper* beekeeper)
: IBeekeeperState(beekeeper)
{
	this->_beekeeper->power_up();
	std::cout << "POWEEERRRR" << std::endl;
	this->_recalculate_path();
	this->_beekeeper->_target_next_vert_in_path();
}


BeekeeperPowerUpState::~BeekeeperPowerUpState()
{
}

void BeekeeperPowerUpState::update(float delta_time)
{
	this->_beekeeper->catchingarea.catch_bees_in_range(this->_max_bees_in_net);

	if (this->_beekeeper->position == this->_beekeeper->target_position) {
		this->_beekeeper->_arrive_at_target(this->_beekeeper->target_vertex);

		if (this->_beekeeper->catchingarea.bees_in_net() >= this->_max_bees_in_net || this->_beekeeper->_graph->bees.size() == 0) {
			this->_set_next_state();
			return;
		}

		this->_recalculate_path();
		if (this->_beekeeper->_path.size() > 0) {
			this->_beekeeper->_target_next_vert_in_path();
		}
	}
	else {
		this->_beekeeper->_step_towards_target(delta_time);
	}
}

void BeekeeperPowerUpState::_recalculate_path()
{
	Vertex* target = this->_get_target_vertex();

	if (target != nullptr) {
		this->_beekeeper->_graph->target = target;
		vector<Vertex*> proposed_path = Graph::find_path(*this->_beekeeper->_graph, this->_beekeeper->current_vertex, target);
		this->_beekeeper->_path = std::deque<Vertex*>(proposed_path.begin(), proposed_path.end());
	}
}

Vertex* BeekeeperPowerUpState::_get_target_vertex()
{
	Bee* closest_bee = this->_beekeeper->_get_closest_bee();

	if (closest_bee != nullptr) {
		Vertex* closest_vert_to_bee = this->_beekeeper->_graph->get_vertex_closest_to_point(closest_bee->pos);

		if (closest_vert_to_bee != nullptr) {
			return closest_vert_to_bee;
		}
		else {
			std::cout << "A bee was found, but there is no closest vertex while recalcing the path for the keeper" << std::endl;
			return nullptr;
		}
	}
	else {
		std::cout << "Not a single bee was found while recalcing the path for the keeper" << std::endl;
		return nullptr;
	}
}

void BeekeeperPowerUpState::_set_next_state()
{
	this->_beekeeper->remove_power_up();
	this->_beekeeper->set_state(new BeekeeperReturnHomeState(this->_beekeeper));
}