#include "BeekeeperPowerUp.h"



BeekeeperPowerUp::BeekeeperPowerUp(Graph& graph)
: _graph(graph)
{
	this->_texture = mApplication->LoadTexture("bee.png");
	this->SetTexture(texture);
	this->SetSize(25, 19);
	mApplication->AddRenderable(this);
	this->Move();
}


BeekeeperPowerUp::~BeekeeperPowerUp()
{
	SDL_DestroyTexture(_texture);
}

void BeekeeperPowerUp::Move()
{

}
