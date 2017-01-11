#pragma once
	#include <deque>
	#include <algorithm>
	#include "../Domain/Graph.h"
	#include "../IGameObject.h"
	#include "../Domain/CoordinateDouble.h"
	#include "CatchingArea.h"
	#include "../States/BeekeeperReturnHomeState.h"
	#include "../States/BeekeeperIdleState.h"
	#include "../States/BeekeeperCatchState.h"
	#include "../States//BeekeeperCatchState.h"

	class Graph;
	struct CoordinateDouble;

	class Beekeeper : public IGameObject 
	{
		friend class CatchingArea;
		friend class IBeekeeperState;
		friend class BeekeeperReturnHomeState;
		friend class BeekeeperIdleState;
		friend class BeekeeperCatchState;
		friend class BeekeeperGetPowerUpState;
	public:
		Beekeeper(Graph* graph, Vertex* start_vertex);
		~Beekeeper();

		CoordinateDouble position;
		CoordinateDouble target_position;

		void Update(float deltaTime) override;
		virtual void Draw() override;

		void set_state(IBeekeeperState* next_state);

		CatchingArea catchingarea;

		Graph* _graph;
		Vertex* current_vertex;
		Vertex* target_vertex = nullptr;
	
	private: 
		const int speed = 700;
		SDL_Texture *texture;

		std::deque<Vertex*> _path;
		IBeekeeperState* _current_state;

		void _step_towards_target(double dt);
		void _apply_vector(Position to_apply);
		Position _get_step_vector(double dt);

		void _target_random_neighbour();
		void _target_vertex(Vertex* v);
		void _target_next_vert_in_path();

		void _arrive_at_target(Vertex* v);

		Bee* _get_closest_bee();

		void _draw_target_vert();
	};

