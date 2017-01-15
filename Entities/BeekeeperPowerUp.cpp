#include "BeekeeperPowerUp.h"
#include <SDL_render.h>

BeekeeperPowerUp::BeekeeperPowerUp(Graph& graph)
: _graph(graph)
{
	this->_texture = mApplication->LoadTexture("mushroom.png");
	this->SetTexture(this->_texture);
	this->SetSize(30, 30);
	mApplication->AddRenderable(this);
	this->Move();
}


BeekeeperPowerUp::~BeekeeperPowerUp()
{
	SDL_DestroyTexture(_texture);
}

void BeekeeperPowerUp::Move()
{
	this->position = this->_graph.getVertexes()[rand() % (int)this->_graph.getVertexes().size()];
}

void BeekeeperPowerUp::Update(float deltaTime)
{
	this->SetOffset((uint32_t)this->position->getXCoord() , (uint32_t)this->position->getYCoord());
}