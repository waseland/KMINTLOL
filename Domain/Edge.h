#pragma once
#include <vector>
#include "Vertex.h"

class Vertex;

class Edge
{
public:
	Edge(Vertex* vertex1, Vertex* vertex2);
	virtual ~Edge();

	Vertex* getVertex1();
	Vertex* getVertex2();

private:
	Vertex* _vertex1;
	Vertex* _vertex2;

};