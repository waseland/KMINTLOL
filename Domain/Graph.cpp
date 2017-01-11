#include "Graph.h"

Graph::Graph()
{
	this->initGraph();
	_beekeeper = new Beekeeper(this, this->_vertexes[109]);
	for (int i = 0; i < this->_num_of_bees; i++) {
		this->bees.push_back(
			new Bee(
				this, 
				0.1,
				{ (double)(rand() % 600), (double)(rand() % 600) }
			)
		);
	}
}


Graph::~Graph()
{
	delete this->_beekeeper;
}

void Graph::addVertex(Vertex* vertex)
{
	this->_vertexes.push_back(vertex);
	vertex->number = (int)this->_vertexes.size() - 1;
}

void Graph::addEdge(Edge* edge)
{
	this->_edges.push_back(edge);
}

vector<Vertex*> Graph::getVertexes()
{
	return this->_vertexes;
}

vector<Edge*> Graph::getEdges()
{
	return this->_edges;
}

void Graph::set_Random_Beekeeper()
{
	int beekeeperindex = rand() % (int)this->getVertexes().size();
	this->_beekeeper->position.x = this->getVertexes().at(beekeeperindex)->getXCoord();
	this->_beekeeper->position.y = this->getVertexes().at(beekeeperindex)->getYCoord();
	this->getVertexes().at(beekeeperindex)->set_beekeeper(this->_beekeeper);
}

Beekeeper* Graph::get_beekeeper()
{
	return this->_beekeeper;
}

float Graph::calculate_raw_distance_between_vertici(Vertex* first, Vertex* second)
{
	int delta_x = second->getXCoord() - first->getXCoord();
	int delta_y = second->getYCoord() - first->getYCoord();

	return (float)sqrt(pow(delta_x, 2) + pow(delta_y, 2));
}

vector<Vertex*> Graph::get_vertex_neighbours(Vertex* start)
{
	vector<Vertex*> neighbours;

	for (Edge* edge : this->_edges) {
		if (edge->getVertex1() == start || edge->getVertex2() == start) {
			// If something goes wrong, look here first
			(edge->getVertex1() == start) ? neighbours.push_back(edge->getVertex2()) : neighbours.push_back(edge->getVertex1());
		}
	}

	return neighbours;
}

Vertex* Graph::get_vertex_closest_to_point(CoordinateDouble pos)
{
	auto vertex_comparison = [pos](Vertex* a, Vertex* b) {

		CoordinateDouble x = { a->getXCoord(), a->getYCoord() };
		CoordinateDouble y = { b->getXCoord(), b->getYCoord() };

		CoordinateDouble a_pos(x);
		CoordinateDouble b_pos(y);

		return a_pos.get_raw_distance_to_point(pos) < b_pos.get_raw_distance_to_point(pos);
	};

	vector<Vertex*>::iterator found_vertex = std::min_element(this->_vertexes.begin(), this->_vertexes.end(), vertex_comparison);

	if (found_vertex != this->_vertexes.end()) {
		return *found_vertex;
	}
	else {
		return nullptr;
	}
}

vector<Vertex*> Graph::find_path(Graph& field, Vertex* start, Vertex* end)
{
	vector<Vertex*> path;

	std::list<ConnectedVertex*> open_list;
	std::list<ConnectedVertex*> closed_list;

	open_list.push_back(new ConnectedVertex{
		start,
		nullptr,
		0,
		0,
		calculate_raw_distance_between_vertici(start, end)
	});

	while (!open_list.empty()) {
		if (open_list.size() > 100) {
			return path;
		}
		ConnectedVertex* q = _get_lowest_cost_vert(open_list);
		open_list.remove(q);
		// Search neighbours (parent becomes q)
		vector<ConnectedVertex*> q_neighbours = _get_astar_neighbours(q, field, end);

		for (ConnectedVertex* astar_neighbour : q_neighbours) {
			if (astar_neighbour->vertex == end) {
				ConnectedVertex* current_vertex = astar_neighbour;

				while (current_vertex->vertex != start) {
					path.push_back(current_vertex->vertex);
					current_vertex = current_vertex->parent;
				}

				return path;
			}

			if (_contains_astar_vector_with_lower_total_cost(open_list, *astar_neighbour)) {
				continue;
			}
			else if (_contains_astar_vector_with_lower_total_cost(closed_list, *astar_neighbour)) {
				continue;
			}

			open_list.push_back(astar_neighbour);
		}

		closed_list.push_back(q);
	}

	return path;
}

ConnectedVertex* Graph::_get_lowest_cost_vert(std::list<ConnectedVertex*>& haystack)
{
	if (haystack.size() <= 0) {
		throw "You're trying to get the lowest cost vert of an empty list";
	}

	ConnectedVertex* lowest = haystack.front();

	for (ConnectedVertex* vert : haystack) {
		if (vert->total_cost < lowest->total_cost) {
			lowest = vert;
		}
	}

	return lowest;
}

vector<ConnectedVertex*> Graph::_get_astar_neighbours(ConnectedVertex* start, Graph& field, Vertex* target)
{
	vector<ConnectedVertex*> astar_neighbours;

	vector<Vertex*> q_neighbours = field.get_vertex_neighbours(start->vertex);
	for (auto vertex : q_neighbours) {
		astar_neighbours.push_back(_make_astar_vertex(vertex, start, target));
	}

	return astar_neighbours;
}

ConnectedVertex* Graph::_make_astar_vertex(Vertex* vert, ConnectedVertex* parent, Vertex* target)
{
	float cost_from_start = parent->cost_from_start + calculate_raw_distance_between_vertici(vert, parent->vertex);
	float cost_to_end = calculate_raw_distance_between_vertici(vert, target);
	float total_cost = cost_from_start + cost_to_end;

	ConnectedVertex* to_return = new ConnectedVertex{
		vert,
		parent,
		total_cost,
		cost_from_start,
		cost_to_end
	};

	if (to_return == to_return->parent) {
		throw "u dun goofed";
	}

	return to_return;
}

bool Graph::_contains_astar_vector_with_lower_total_cost(std::list<ConnectedVertex*>& haystack, ConnectedVertex& needle)
{
	for (ConnectedVertex* astar_vector : haystack) {
		if (haystack.size() == 0) {
			throw "Done";
		}

		if (astar_vector->vertex->number == needle.vertex->number) {
			if (astar_vector->total_cost < needle.total_cost) {
				return true;
			}
		}

		/*if (astar_vector->vertex->getXCoord() == needle.vertex->getXCoord()
			&& astar_vector->vertex->getYCoord() == needle.vertex->getYCoord()) {

			if (astar_vector->total_cost < needle.total_cost) {
				return true;
			}
		}*/
	}

	return false;
} 

void Graph::Update()
{
	if (this->bees_out_of_game.size() == this->_num_of_bees) {
		this->_reset_bees();
	}
}

void Graph::_reset_bees()
{
	double total_vision = 0;
	double total_speed = 0;
	for (Bee* bee : this->bees_out_of_game) {
		total_vision += bee->max_vision_range;
		total_speed += bee->max_speed;
	}
	double average_vision = total_vision / this->bees_out_of_game.size();
	double average_speed = total_speed / this->bees_out_of_game.size();

	for (Bee* bee : this->bees_out_of_game) {
		bee->SetActive(true);
		bee->pos = { (double)(rand() % 600), (double)(rand() % 600) };

		//generic algoritm

		double vision = (double)(rand() % 100) / 1000 + 1;
		double speed = (double)(rand() % 100) / 1000 + 1;

		bee->max_vision_range = average_vision * vision;
		bee->max_speed = average_speed * speed;
		
		this->bees.push_back(bee);
	}

	this->bees_out_of_game.clear();
}


void Graph::initGraph() 
{
	auto vertex103 = new Vertex(28, 31);
	this->addVertex(vertex103);
	auto vertex104 = new Vertex(60, 49);
	this->addVertex(vertex104);
	auto vertex105 = new Vertex(15, 76);
	this->addVertex(vertex105);
	auto vertex106 = new Vertex(39, 96);
	this->addVertex(vertex106);
	auto vertex107 = new Vertex(141, 97);
	this->addVertex(vertex107);
	auto vertex108 = new Vertex(129, 30);
	this->addVertex(vertex108);
	auto vertex109 = new Vertex(186, 64);
	this->addVertex(vertex109);
	auto vertex110 = new Vertex(201, 15);
	this->addVertex(vertex110);
	auto vertex111 = new Vertex(258, 49);
	this->addVertex(vertex111);
	auto vertex112 = new Vertex(213, 78);
	this->addVertex(vertex112);
	auto vertex113 = new Vertex(241, 94);
	this->addVertex(vertex113);
	auto vertex114 = new Vertex(216, 106);
	this->addVertex(vertex114);
	auto vertex115 = new Vertex(312, 157);
	this->addVertex(vertex115);
	auto vertex116 = new Vertex(289, 172);
	this->addVertex(vertex116);
	auto vertex117 = new Vertex(444, 235);
	this->addVertex(vertex117);
	auto vertex118 = new Vertex(487, 202);
	this->addVertex(vertex118);
	auto vertex119 = new Vertex(457, 186);
	this->addVertex(vertex119);
	auto vertex120 = new Vertex(483, 169);
	this->addVertex(vertex120);
	auto vertex121 = new Vertex(526, 138);
	this->addVertex(vertex121);
	auto vertex122 = new Vertex(556, 156);
	this->addVertex(vertex122);
	auto vertex123 = new Vertex(435, 96);
	this->addVertex(vertex123);
	auto vertex124 = new Vertex(408, 82);
	this->addVertex(vertex124);
	auto vertex125 = new Vertex(488, 61);
	this->addVertex(vertex125);
	auto vertex126 = new Vertex(399, 121);
	this->addVertex(vertex126);
	auto vertex127 = new Vertex(367, 108);
	this->addVertex(vertex127);
	auto vertex128 = new Vertex(346, 122);
	this->addVertex(vertex128);
	auto vertex129 = new Vertex(288, 91);
	this->addVertex(vertex129);
	auto vertex130 = new Vertex(351, 46);
	this->addVertex(vertex130);
	auto vertex131 = new Vertex(301, 15);
	this->addVertex(vertex131);
	auto vertex132 = new Vertex(402, 15);
	this->addVertex(vertex132);
	auto vertex133 = new Vertex(547, 18);
	this->addVertex(vertex133);
	auto vertex134 = new Vertex(531, 33);
	this->addVertex(vertex134);
	auto vertex135 = new Vertex(562, 49);
	this->addVertex(vertex135);
	auto vertex136 = new Vertex(541, 64);
	this->addVertex(vertex136);
	auto vertex137 = new Vertex(567, 79);
	this->addVertex(vertex137);
	auto vertex138 = new Vertex(544, 93);
	this->addVertex(vertex138);
	auto vertex139 = new Vertex(574, 112);
	this->addVertex(vertex139);
	auto vertex140 = new Vertex(568, 220);
	this->addVertex(vertex140);
	auto vertex141 = new Vertex(570, 250);
	this->addVertex(vertex141);
	auto vertex142 = new Vertex(543, 234);
	this->addVertex(vertex142);
	auto vertex143 = new Vertex(499, 264);
	this->addVertex(vertex143);
	auto vertex144 = new Vertex(553, 297);
	this->addVertex(vertex144);
	auto vertex145 = new Vertex(486, 341);
	this->addVertex(vertex145);
	auto vertex146 = new Vertex(430, 310);
	this->addVertex(vertex146);
	auto vertex147 = new Vertex(406, 327);
	this->addVertex(vertex147);
	auto vertex148 = new Vertex(376, 307);
	this->addVertex(vertex148);
	auto vertex149 = new Vertex(331, 345);
	this->addVertex(vertex149);
	auto vertex150 = new Vertex(297, 295);
	this->addVertex(vertex150);
	auto vertex151 = new Vertex(255, 325);
	this->addVertex(vertex151);
	auto vertex152 = new Vertex(274, 312);
	this->addVertex(vertex152);
	auto vertex153 = new Vertex(216, 280);
	this->addVertex(vertex153);
	auto vertex154 = new Vertex(147, 328);
	this->addVertex(vertex154);
	auto vertex155 = new Vertex(264, 393);
	this->addVertex(vertex155);
	auto vertex156 = new Vertex(229, 170);
	this->addVertex(vertex156);
	auto vertex157 = new Vertex(201, 154);
	this->addVertex(vertex157);
	auto vertex158 = new Vertex(178, 174);
	this->addVertex(vertex158);
	auto vertex159 = new Vertex(151, 156);
	this->addVertex(vertex159);
	auto vertex160 = new Vertex(162, 187);
	this->addVertex(vertex160);
	auto vertex161 = new Vertex(135, 201);
	this->addVertex(vertex161);
	auto vertex162 = new Vertex(108, 183);
	this->addVertex(vertex162);
	auto vertex163 = new Vertex(13, 187);
	this->addVertex(vertex163);
	auto vertex164 = new Vertex(144, 267);
	this->addVertex(vertex164);
	auto vertex165 = new Vertex(16, 357);
	this->addVertex(vertex165);
	auto vertex166 = new Vertex(70, 391);
	this->addVertex(vertex166);
	auto vertex167 = new Vertex(40, 411);
	this->addVertex(vertex167);
	auto vertex168 = new Vertex(61, 421);
	this->addVertex(vertex168);
	auto vertex169 = new Vertex(33, 438);
	this->addVertex(vertex169);
	auto vertex170 = new Vertex(63, 450);
	this->addVertex(vertex170);
	auto vertex171 = new Vertex(37, 466);
	this->addVertex(vertex171);
	auto vertex172 = new Vertex(97, 499);
	this->addVertex(vertex172);
	auto vertex173 = new Vertex(36, 544);
	this->addVertex(vertex173);
	auto vertex174 = new Vertex(109, 558);
	this->addVertex(vertex174);
	auto vertex175 = new Vertex(57, 528);
	this->addVertex(vertex175);
	auto vertex176 = new Vertex(156, 528);
	this->addVertex(vertex176);
	auto vertex177 = new Vertex(142, 472);
	this->addVertex(vertex177);
	auto vertex178 = new Vertex(112, 453);
	this->addVertex(vertex178);
	auto vertex179 = new Vertex(181, 402);
	this->addVertex(vertex179);
	auto vertex180 = new Vertex(244, 438);
	this->addVertex(vertex180);
	auto vertex181 = new Vertex(291, 406);
	this->addVertex(vertex181);
	auto vertex182 = new Vertex(387, 373);
	this->addVertex(vertex182);
	auto vertex183 = new Vertex(415, 391);
	this->addVertex(vertex183);
	auto vertex184 = new Vertex(387, 408);
	this->addVertex(vertex184);
	auto vertex185 = new Vertex(316, 451);
	this->addVertex(vertex185);
	auto vertex186 = new Vertex(442, 438);
	this->addVertex(vertex186);
	auto vertex187 = new Vertex(465, 421);
	this->addVertex(vertex187);
	auto vertex188 = new Vertex(487, 400);
	this->addVertex(vertex188);
	auto vertex189 = new Vertex(493, 436);
	this->addVertex(vertex189);
	auto vertex190 = new Vertex(540, 438);
	this->addVertex(vertex190);
	auto vertex191 = new Vertex(571, 424);
	this->addVertex(vertex191);
	auto vertex192 = new Vertex(543, 406);
	this->addVertex(vertex192);
	auto vertex193 = new Vertex(571, 394);
	this->addVertex(vertex193);
	auto vertex194 = new Vertex(538, 372);
	this->addVertex(vertex194);
	auto vertex195 = new Vertex(564, 358);
	this->addVertex(vertex195);
	auto vertex196 = new Vertex(351, 267);
	this->addVertex(vertex196);
	auto vertex197 = new Vertex(373, 249);
	this->addVertex(vertex197);
	auto vertex198 = new Vertex(402, 265);
	this->addVertex(vertex198);
	auto vertex199 = new Vertex(294, 232);
	this->addVertex(vertex199);
	auto vertex200 = new Vertex(316, 219);
	this->addVertex(vertex200);
	auto vertex201 = new Vertex(264, 216);
	this->addVertex(vertex201);
	auto vertex202 = new Vertex(238, 237);
	this->addVertex(vertex202);
	auto vertex203 = new Vertex(211, 219);
	this->addVertex(vertex203);
	auto vertex204 = new Vertex(352, 577);
	this->addVertex(vertex204);
	auto vertex205 = new Vertex(261, 529);
	this->addVertex(vertex205);
	auto vertex206 = new Vertex(303, 499);
	this->addVertex(vertex206);
	auto vertex207 = new Vertex(245, 469);
	this->addVertex(vertex207);
	auto vertex211 = new Vertex(87, 61);
	this->addVertex(vertex211);
	auto vertex306 = new Vertex(345, 468);
	this->addVertex(vertex306);
	auto vertex322 = new Vertex(213, 562);
	this->addVertex(vertex322);
	auto vertex325 = new Vertex(190, 576);
	this->addVertex(vertex325);
	auto vertex327 = new Vertex(384, 555); // Dit is de basis rechtsonder
	this->addVertex(vertex327);
	this->addEdge(new Edge(vertex103, vertex104));
	this->addEdge(new Edge(vertex106, vertex105));
	this->addEdge(new Edge(vertex104, vertex105));
	this->addEdge(new Edge(vertex211, vertex104));
	this->addEdge(new Edge(vertex211, vertex108));
	this->addEdge(new Edge(vertex211, vertex107));
	this->addEdge(new Edge(vertex107, vertex109));
	this->addEdge(new Edge(vertex109, vertex112));
	this->addEdge(new Edge(vertex112, vertex111));
	this->addEdge(new Edge(vertex111, vertex111));
	this->addEdge(new Edge(vertex113, vertex112));
	this->addEdge(new Edge(vertex113, vertex114));
	this->addEdge(new Edge(vertex114, vertex115));
	this->addEdge(new Edge(vertex115, vertex116));
	this->addEdge(new Edge(vertex115, vertex117));
	this->addEdge(new Edge(vertex117, vertex198));
	this->addEdge(new Edge(vertex198, vertex197));
	this->addEdge(new Edge(vertex197, vertex196));
	this->addEdge(new Edge(vertex196, vertex199));
	this->addEdge(new Edge(vertex199, vertex200));
	this->addEdge(new Edge(vertex199, vertex201));
	this->addEdge(new Edge(vertex201, vertex202));
	this->addEdge(new Edge(vertex202, vertex203));
	this->addEdge(new Edge(vertex203, vertex160));
	this->addEdge(new Edge(vertex160, vertex158));
	this->addEdge(new Edge(vertex158, vertex159));
	this->addEdge(new Edge(vertex158, vertex157));
	this->addEdge(new Edge(vertex157, vertex156));
	this->addEdge(new Edge(vertex160, vertex161));
	this->addEdge(new Edge(vertex161, vertex162));
	this->addEdge(new Edge(vertex107, vertex163));
	this->addEdge(new Edge(vertex163, vertex164));
	this->addEdge(new Edge(vertex129, vertex130));
	this->addEdge(new Edge(vertex128, vertex129));
	this->addEdge(new Edge(vertex128, vertex127));
	this->addEdge(new Edge(vertex127, vertex126));
	this->addEdge(new Edge(vertex126, vertex123));
	this->addEdge(new Edge(vertex123, vertex124));
	this->addEdge(new Edge(vertex123, vertex125));
	this->addEdge(new Edge(vertex134, vertex135));
	this->addEdge(new Edge(vertex135, vertex136));
	this->addEdge(new Edge(vertex136, vertex137));
	this->addEdge(new Edge(vertex137, vertex138));
	this->addEdge(new Edge(vertex138, vertex139));
	this->addEdge(new Edge(vertex139, vertex121));
	this->addEdge(new Edge(vertex121, vertex122));
	this->addEdge(new Edge(vertex121, vertex123));
	this->addEdge(new Edge(vertex121, vertex120));
	this->addEdge(new Edge(vertex120, vertex119));
	this->addEdge(new Edge(vertex119, vertex118));
	this->addEdge(new Edge(vertex118, vertex117));
	this->addEdge(new Edge(vertex118, vertex142));
	this->addEdge(new Edge(vertex142, vertex140));
	this->addEdge(new Edge(vertex142, vertex141));
	this->addEdge(new Edge(vertex142, vertex143));
	this->addEdge(new Edge(vertex143, vertex144));
	this->addEdge(new Edge(vertex144, vertex145));
	this->addEdge(new Edge(vertex145, vertex146));
	this->addEdge(new Edge(vertex146, vertex147));
	this->addEdge(new Edge(vertex147, vertex148));
	this->addEdge(new Edge(vertex148, vertex149));
	this->addEdge(new Edge(vertex149, vertex152));
	this->addEdge(new Edge(vertex152, vertex150));
	this->addEdge(new Edge(vertex152, vertex151));
	this->addEdge(new Edge(vertex152, vertex153));
	this->addEdge(new Edge(vertex153, vertex154));
	this->addEdge(new Edge(vertex154, vertex166));
	this->addEdge(new Edge(vertex166, vertex165));
	this->addEdge(new Edge(vertex165, vertex164));
	this->addEdge(new Edge(vertex164, vertex203));
	this->addEdge(new Edge(vertex172, vertex171));
	this->addEdge(new Edge(vertex171, vertex170));
	this->addEdge(new Edge(vertex170, vertex169));
	this->addEdge(new Edge(vertex169, vertex168));
	this->addEdge(new Edge(vertex167, vertex166));
	this->addEdge(new Edge(vertex168, vertex167));
	this->addEdge(new Edge(vertex173, vertex175));
	this->addEdge(new Edge(vertex172, vertex175));
	this->addEdge(new Edge(vertex177, vertex172));
	this->addEdge(new Edge(vertex178, vertex177));
	this->addEdge(new Edge(vertex178, vertex179));
	this->addEdge(new Edge(vertex179, vertex180));
	this->addEdge(new Edge(vertex155, vertex181));
	this->addEdge(new Edge(vertex155, vertex149));
	this->addEdge(new Edge(vertex155, vertex154));
	this->addEdge(new Edge(vertex183, vertex182));
	this->addEdge(new Edge(vertex183, vertex184));
	this->addEdge(new Edge(vertex183, vertex145));
	this->addEdge(new Edge(vertex145, vertex194));
	this->addEdge(new Edge(vertex194, vertex195));
	this->addEdge(new Edge(vertex194, vertex193));
	this->addEdge(new Edge(vertex193, vertex192));
	this->addEdge(new Edge(vertex192, vertex191));
	this->addEdge(new Edge(vertex188, vertex187));
	this->addEdge(new Edge(vertex184, vertex185));
	this->addEdge(new Edge(vertex205, vertex204));
	this->addEdge(new Edge(vertex205, vertex206));
	this->addEdge(new Edge(vertex185, vertex306));
	this->addEdge(new Edge(vertex206, vertex306));
	this->addEdge(new Edge(vertex207, vertex206));
	this->addEdge(new Edge(vertex176, vertex207));
	this->addEdge(new Edge(vertex176, vertex174));
	this->addEdge(new Edge(vertex174, vertex175));
	this->addEdge(new Edge(vertex187, vertex186));
	this->addEdge(new Edge(vertex187, vertex189));
	this->addEdge(new Edge(vertex186, vertex184));
	this->addEdge(new Edge(vertex191, vertex190));
	this->addEdge(new Edge(vertex111, vertex110));
	this->addEdge(new Edge(vertex130, vertex131));
	this->addEdge(new Edge(vertex130, vertex132));
	this->addEdge(new Edge(vertex132, vertex125));
	this->addEdge(new Edge(vertex133, vertex134));
	this->addEdge(new Edge(vertex322, vertex176));
	this->addEdge(new Edge(vertex180, vertex181));
	this->addEdge(new Edge(vertex325, vertex322));
	this->addEdge(new Edge(vertex204, vertex327));
}

