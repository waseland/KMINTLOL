#include <iostream>
#include "BeekeeperCatchState.h"
#include "../Entities/Beekeeper.h"
#include "../Domain/Graph.h"

BeekeeperCatchState::BeekeeperCatchState(Beekeeper* beekeeper)
	: IBeekeeperState(beekeeper)
{
	this->name = "Beekeeper catch state";
	std::cout << "Catching bee's" << std::endl;

	this->_recalculate_path();
	this->_beekeeper->_target_next_vert_in_path();
}

void BeekeeperCatchState::update(float delta_time)
{
	// std::cout << "pos [" << this->_beekeeper->current_position.x << "," << this->_beekeeper->current_position.y << "]" << std::endl;
	// std::cout << "target [" << this->_beekeeper->target_position.x << "," << this->_beekeeper->target_position.y << "]" << std::endl;

	this->_beekeeper->catchingarea.catch_bees_in_range(this->_max_bees_in_net);

	if (this->_beekeeper->position == this->_beekeeper->target_position) {
		this->_beekeeper->_arrive_at_target(this->_beekeeper->target_vertex);

		if (this->_beekeeper->catchingarea.bees_in_net() >= this->_max_bees_in_net) {
			this->_set_next_state();
			return;
		}

		this->_recalculate_path();
		// Target the next thingy in the path
		if (this->_beekeeper->_path.size() > 0) {
			this->_beekeeper->_target_next_vert_in_path();
		}
	}
	else {
		this->_beekeeper->_step_towards_target(delta_time);
	}
}

void BeekeeperCatchState::_recalculate_path()
{
	Vertex* target = this->_get_target_vertex();

	if (target != nullptr) {
		this->_beekeeper->_graph->target = target;
		vector<Vertex*> proposed_path = Graph::find_path(*this->_beekeeper->_graph, this->_beekeeper->current_vertex, target);
		this->_beekeeper->_path = std::deque<Vertex*>(proposed_path.begin(), proposed_path.end());
	}
}

Vertex* BeekeeperCatchState::_get_target_vertex()
{
	Bee* closest_bee = this->_beekeeper->_get_closest_bee();
    //std::cout << closest_bee->pos.x << " , " << closest_bee->pos.y << std::endl;

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

void BeekeeperCatchState::_set_next_state()
{
	    // TODO: Make this an autistical FSM

	int random = this->_beekeeper->_graph->fsm->fsm_get_state();
	switch (random) {
	case 1:
		this->_beekeeper->set_state(new BeekeeperReturnHomeState(this->_beekeeper));
		break;
	case 2:
		this->_beekeeper->set_state(new BeekeeperGetPowerUpState(this->_beekeeper));		
		break;
	case 3:
		this->_beekeeper->set_state(new BeekeeperIdleState(this->_beekeeper));
		break;
	default:
		this->_set_next_state();
		return;
		break;
	}
	this->_beekeeper->_graph->fsm->recalculate_states();

	/*int next_state_choice = rand() % 3;
	if (next_state_choice == 0) {
		this->_beekeeper->set_state(new BeekeeperReturnHomeState(this->_beekeeper));
		
	}
	else if (next_state_choice == 1) {
		this->_beekeeper->set_state(new BeekeeperIdleState(this->_beekeeper));
		
	}
	else if (next_state_choice == 2) {
		this->_beekeeper->set_state(new BeekeeperGetPowerUpState(this->_beekeeper));
	}*/
	//if (this->_context.field.bees.size() > 0) {
	//	this->_context.set_state(this->_context.fsm.get_next_state());
	//}
	//else {
	//	this->_context.set_state(new BeeKeeperReturnState(this->_context));
	//};
}