#pragma once

#include "../IGameObject.h"
#include "../Domain/Graph.h"

class Vertex;

class BeekeeperPowerUp : IGameObject
{
public:
	BeekeeperPowerUp(Graph& graph);
	~BeekeeperPowerUp();

	Vertex* position;

	void Move();

	virtual void Update(float deltaTime) override;

private:
	SDL_Texture* _texture;
	Graph& _graph;
};

