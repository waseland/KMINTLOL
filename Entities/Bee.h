#pragma once

#include "../IGameObject.h"
#include "../Domain/Graph.h"
#include "../Vector/Position.h"
#include "../Domain/CoordinateDouble.h"

class Graph;
class CatchingArea;

class Bee : public IGameObject
{
	friend class Movement;
	public:
		Bee(Graph* graph, double maxspeed, CoordinateDouble position );
		~Bee();
		virtual void Update(float deltaTime) override;

		virtual void Draw() override;

		CoordinateDouble pos;
		const int radius = 20;
		double max_speed;
		double max_vision_range = 50;
		double bee_evade_range = 15;
		double max_force = 8;
		Position velocity;
		Position heading;
		Position side;
		Graph* _graph;

		void get_caught(CatchingArea& net);
		void get_set_free(CatchingArea& net);

	private:
		int x_accel = 1;
		int y_axxel = 1;
		SDL_Texture *texture;

		double wander_radius = 3;
		double wander_distance = 2.5;
		double wander_jitter = 0.5;
		Position wander_target;

		double weight = 0.1;

		Movement* behaviour;

		void Move(int delta_time);
};

