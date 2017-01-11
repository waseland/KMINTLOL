#include "Edge.h"



Edge::Edge(Vertex* vertex1, Vertex* vertex2)
	: _vertex1(vertex1)
	, _vertex2(vertex2)
{
	vertex1->addEdgeToVertex(this);
	vertex2->addEdgeToVertex(this);
}


Edge::~Edge()
{
}

Vertex* Edge::getVertex1()
{
	return this->_vertex1;
}
Vertex* Edge::getVertex2()
{
	return this->_vertex2;
}