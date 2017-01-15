#pragma once

#include <vector>
#include "Vertex.h"
#include "Edge.h"
#include <stdlib.h>
#include <cstdlib>
#include <cmath>
#include <iostream>
#include "../Entities/Beekeeper.h"
#include "../Entities/Bee.h"
#include "ConnectedVertex.h"
#include <list>
#include "../FSM/FSM.h"

using std::vector;

class Beekeeper;
class Vertex;
class Edge;
class Bee;
struct ConnectedVertex;
class BeekeeperPowerUp;

class Graph
{

public:
	Graph();
	~Graph();

	void addVertex(Vertex* vertex);
	void addEdge(Edge* edge);
	vector<Vertex*> getVertexes();
	vector<Edge*> getEdges();
	void set_Random_Beekeeper();
	void initGraph();
	void Update();
	Beekeeper* get_beekeeper();
	vector<Bee*> bees;
	vector<Bee*> bees_out_of_game;

	vector<Vertex*> get_vertex_neighbours(Vertex* start);

	static vector<Vertex*> find_path(Graph& field, Vertex* start, Vertex* end);
	
	static float calculate_raw_distance_between_vertici(Vertex* first, Vertex* second);
	Vertex* get_vertex_closest_to_point(CoordinateDouble pos);

	BeekeeperPowerUp* beekeeper_power_up; 
	Vertex* target = nullptr;
	FSM* fsm;

private:

	const int _num_of_bees = 80;
	Beekeeper* _beekeeper;
	vector<Vertex*> _vertexes;
	vector<Edge*> _edges;

	void _reset_bees();

	static ConnectedVertex* _get_lowest_cost_vert(std::list<ConnectedVertex*>& haystack);
	static vector<ConnectedVertex*> _get_astar_neighbours(ConnectedVertex* start, Graph& field, Vertex* target);
	static ConnectedVertex* _make_astar_vertex(Vertex* vert, ConnectedVertex* parent, Vertex* target);
	static bool _contains_astar_vector_with_lower_total_cost(std::list<ConnectedVertex*>& haystack, ConnectedVertex& needle);

};

