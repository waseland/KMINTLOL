#include "BeekeeperReturnHomeState.h"
#include "../Domain/Graph.h"
#include "../Entities/Beekeeper.h"

BeekeeperReturnHomeState::BeekeeperReturnHomeState(Beekeeper* beekeeper)
	: IBeekeeperState(beekeeper)
{
	std::cout << "Returning home" << std::endl;
	this->_set_path_to_home();
}

void BeekeeperReturnHomeState::update(float delta_time)
{
	if (this->_beekeeper->position == this->_beekeeper->target_position) {
		this->_beekeeper->_arrive_at_target(this->_beekeeper->target_vertex);
		// Target the next thingy in the path
		if (this->_beekeeper->_path.size() > 0) {
			this->_beekeeper->_target_next_vert_in_path();
		}
		else {
			std::cout << "The beekeeper returned home!" << std::endl;
			this->_returned_home();
			this->_beekeeper->set_state(new BeekeeperCatchState(this->_beekeeper));
		}
	}
	else {
		this->_beekeeper->_step_towards_target(delta_time);
	}
}

void BeekeeperReturnHomeState::_set_path_to_home()
{
	this->_beekeeper->_graph->target = this->_beekeeper->_graph->getVertexes().at(this->_beekeeper->_graph->getVertexes().size() - 1);
	vector<Vertex*> proposed_path = Graph::find_path(*this->_beekeeper->_graph, this->_beekeeper->current_vertex, this->_beekeeper->_graph->getVertexes().at(this->_beekeeper->_graph->getVertexes().size()-1));
	this->_beekeeper->_path = std::deque<Vertex*>(proposed_path.begin(), proposed_path.end());
}

void BeekeeperReturnHomeState::_returned_home()
{
	//this->_beekeeper->fsm.adjust_fsmlets(this->_context.net.caught_bees.size());
	size_t spend_ticks = _beekeeper->mApplication->get_ticks() - _beekeeper->previous_ticks;
	this->_beekeeper->_graph->fsm->recalculate_states(spend_ticks, this->_beekeeper->catchingarea.caught_bees.size());
	this->_beekeeper->catchingarea.clear();
	this->_beekeeper->catchingarea.reset_size();
	this->_beekeeper->previous_ticks = this->_beekeeper->mApplication->get_ticks();
}