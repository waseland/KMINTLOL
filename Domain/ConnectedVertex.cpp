#include "ConnectedVertex.h"

ConnectedVertex::ConnectedVertex(Vertex* vertex, ConnectedVertex* parent, float total_cost, float cost_from_start,
	float cost_to_end)
	: vertex(vertex)
	, parent(parent)
	, total_cost(total_cost)
	, cost_from_start(cost_from_start)
	, cost_to_end(cost_to_end)
{
}