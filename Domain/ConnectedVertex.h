#pragma once

#include "Vertex.h"

class Vertex;

struct ConnectedVertex {
public:
	ConnectedVertex(Vertex* vertex, ConnectedVertex* parent, float total_cost, float cost_from_start, float cost_to_end);

	Vertex* vertex;
	ConnectedVertex* parent;

	float total_cost;           // f
	float cost_from_start;      // g
	float cost_to_end;          // h (estimated)

	bool operator==(const ConnectedVertex& other) {
		return this->vertex == other.vertex
			&& this->parent == other.parent;
	}
};


