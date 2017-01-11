#pragma once

#include <vector>
#include "Edge.h"
#include "../Entities/Beekeeper.h"

using std::vector;

class Edge;
class Beekeeper;

class Vertex
{
public:
	Vertex(int xCoord, int yCoord);
	~Vertex();

	int number;

	void addEdgeToVertex(Edge* edge);

	int getXCoord();
	int getYCoord();

	vector<Edge*> getNeighBourEdges();

	void set_beekeeper(Beekeeper* beekeeper);
	Beekeeper* get_beekeeper();

private:
	int _xCoord;
	int _yCoord;
	vector<Edge*> _neighbourEdges;

	Beekeeper* _beekeeper;
};

