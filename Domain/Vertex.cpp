#include "Vertex.h"

Vertex::Vertex(int xCoord, int yCoord)
{
	this->_xCoord = xCoord;
	this->_yCoord = yCoord;
}


Vertex::~Vertex()
{
}

void Vertex::addEdgeToVertex(Edge* edge)
{
	this->_neighbourEdges.push_back(edge);
}

int Vertex::getXCoord()
{
	return this->_xCoord;
}

int Vertex::getYCoord()
{
	return _yCoord;
}

vector<Edge*> Vertex::getNeighBourEdges()
{
	return this->_neighbourEdges;
}

void Vertex::set_beekeeper(Beekeeper* beekeeper)
{
	this->_beekeeper = beekeeper;
}

Beekeeper* Vertex::get_beekeeper()
{
	return this->_beekeeper;
}
