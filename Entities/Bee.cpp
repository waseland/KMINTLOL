#include "Bee.h"
#include <SDL_render.h>
#include "Movement.h"
#include <algorithm>
#include "CatchingArea.h"

Bee::Bee(Graph* graph, double maxspeed, CoordinateDouble position)
: _graph(graph)
, pos(position)
, max_speed(maxspeed)
, behaviour(new Movement(*this))
{
	texture = mApplication->LoadTexture("bee.png");
	this->SetTexture(texture);
	this->SetSize(25, 19);
	mApplication->AddRenderable(this);
	//this->flying_left = (rand() % 2 == 1);
}


Bee::~Bee()
{
	SDL_DestroyTexture(texture);
}

void Bee::Update(float deltaTime)
{
	if (this->mIsActive) {
		this->Move(deltaTime * 1000);
		this->SetOffset((uint32_t)this->pos.x, (uint32_t)this->pos.y);
	}
}

void Bee::Draw()
{
	if (this->mIsActive) {
		IGameObject::Draw();
	}
}

void Bee::Move(int delta_time)
{
	Position steering_force = behaviour->check_movement();
	Position accel = steering_force / this->weight;

	this->velocity = this->velocity + (accel * delta_time); 
	if (this->velocity.get_length() > 0.00000001) { 
		this->velocity = this->velocity.truncate(this->max_speed);
	}

	Position delta_pos = this->velocity * delta_time;
	this->pos = this->pos + delta_pos;

	if (this->velocity.get_length() > 0.00000001) { 
		this->heading = this->velocity.normalise();
		this->side = this->heading.perpendicular();
	}

	this->pos = this->pos.wrap(600, 600);
}

void Bee::get_caught(CatchingArea& net)
{
	this->SetActive(false);

	// Move bee from BeeField's bees to the net
	this->_graph->bees.erase(
		std::remove(this->_graph->bees.begin(), this->_graph->bees.end(), this),
		this->_graph->bees.end()
	);

	net.caught_bees.push_back(this);
}

void Bee::get_set_free(CatchingArea& net)
{
	this->SetActive(true);

	// Set our position to the net's position
	this->pos = net.get_position();

	// Move bee from net to Beefield's bees
	this->_graph->bees.push_back(this);

	net.caught_bees.erase(
		std::remove(net.caught_bees.begin(), net.caught_bees.end(), this),
		net.caught_bees.end()
	);
}