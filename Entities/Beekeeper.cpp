#include "Beekeeper.h"
#include <SDL_render.h>

Beekeeper::Beekeeper(Graph* graph, Vertex* start_vertex)
: _graph(graph)
, current_vertex(start_vertex)
, target_vertex(current_vertex)
, position(start_vertex->getXCoord(), start_vertex->getYCoord())
, target_position(start_vertex->getXCoord(), start_vertex->getYCoord())
, catchingarea(this)
{
	texture = mApplication->LoadTexture("beekeeper.png");
	this->SetTexture(texture);
	this->SetSize(52, 75);
	mApplication->AddRenderable(this);

	this->set_state(new BeekeeperCatchState(this));
}


Beekeeper::~Beekeeper()
{
	SDL_DestroyTexture(texture);
}

void Beekeeper::Update(float deltaTime)
{
	this->_current_state->update(deltaTime);

	this->SetOffset((uint32_t)this->position.x, (uint32_t)this->position.y);

}

void Beekeeper::Draw()
{
	IGameObject::Draw();
}

void Beekeeper::_step_towards_target(double dt)
{
	Position step_vector = this->_get_step_vector(dt);
	Position to_apply = step_vector;
	this->_apply_vector(to_apply);
}

Position Beekeeper::_get_step_vector(double dt)
{
	Position step_vector(this->position - this->target_position);
	Position kill_me = step_vector.normalise() * speed * dt;

	// Check if total length to dest is higher than the length that we're about to take
	if (step_vector.get_length() > kill_me.get_length()) {
		return kill_me;
	}
	else {
		return step_vector;
	}
}

void Beekeeper::_apply_vector(Position to_apply)
{
	this->position = this->position - to_apply;
}

void Beekeeper::_target_random_neighbour()
{
	vector<Vertex*> neighbours = this->_graph->get_vertex_neighbours(this->current_vertex);

	if (neighbours.size() > 0) {
		int index = rand() % (neighbours.size() - 1);
		this->target_vertex = neighbours[index];
	}
}

void Beekeeper::_target_next_vert_in_path()
{
	// Move to the next vertex in our path
	if (!this->_path.empty()) {
		Vertex* move_here = this->_path.back();
		this->_path.pop_back();

		this->_target_vertex(move_here);
	}
}

void Beekeeper::_target_vertex(Vertex* v)
{
	this->target_vertex = v;
	this->target_position = {(double)v->getXCoord(), (double)v->getYCoord()};
}

void Beekeeper::_arrive_at_target(Vertex* v)
{
	this->current_vertex = v;
}

/// If there are no bees, a nullptr is returned
Bee* Beekeeper::_get_closest_bee()
{
	auto bee_comparison = [this](Bee* a, Bee* b) {
		CoordinateDouble& pos = this->position;

		return a->pos.get_raw_distance_to_point(pos) < b->pos.get_raw_distance_to_point(pos);
	};

	vector<Bee*>::iterator found_bee = std::min_element(this->_graph->bees.begin(), this->_graph->bees.end(), bee_comparison);

	if (found_bee != this->_graph->bees.end()) {
		return *found_bee;
	}
	else {
		return nullptr;
	}
}

void Beekeeper::_draw_target_vert()
{
	if (this->_path.size() > 0) {
		this->mApplication->SetColor(Color{ 0, 255, 0, 255 });

		Vertex* last_vert_in_path = this->_path.front();
		this->mApplication->DrawCircle(last_vert_in_path->getXCoord(), last_vert_in_path->getYCoord(), 8, true);
	}
}

void Beekeeper::set_state(IBeekeeperState* next_state)
{
	this->_current_state = next_state;
}