#include "CatchingArea.h"
#include "Beekeeper.h"
#include "../Vector/Sphere.h"
#include <algorithm>



CatchingArea::CatchingArea(Beekeeper* context)
: _beekeeper(context)
, _radius(this->_net_start_radius)
, _last_growth_timestamp(this->mApplication->GetTimeSinceStartedMS())
{
	mApplication->AddRenderable(this);
}

void CatchingArea::Draw()
{
	this->mApplication->SetColor(this->_draw_colour);
	this->mApplication->DrawCircle(
		(int)this->_beekeeper->position.x,
		(int)this->_beekeeper->position.y,
		this->_radius, true
	);
}

void CatchingArea::Update(float deltaTime)
{
	int now = this->mApplication->GetTimeSinceStartedMS();

	if (now > this->_last_growth_timestamp + this->_net_growth_interval_ms) {
		this->_expand();
	}
}

void CatchingArea::_expand()
{
	this->_last_growth_timestamp = this->mApplication->GetTimeSinceStartedMS();
	this->_radius += this->_net_growth;
}

void CatchingArea::catch_bees_in_range(int max_bees_in_net)
{
	int num_of_bees_to_catch = max_bees_in_net - this->bees_in_net();

	if (num_of_bees_to_catch < 0) {
		// Something is horribly wrong lol
		return;
	}

	vector<Bee*> bees_in_range = this->_get_bees_in_net_range();
	// I probably should have solved this the imperative way instead of making it a magic one-liner.
	// But what this basically does is initialise bees_to_catch with the amount of bees that we want to catch.
	// In the case that bees_in_range is smaller than num_of_bees... then it will be a copy of bees_in_range.
	// Otherwise it will be the first num_of_bees... elements of bees_in_range.
	vector<Bee*> bees_to_catch(
		bees_in_range.begin(),
		std::next(
			bees_in_range.begin(),
			std::min((size_t)num_of_bees_to_catch, bees_in_range.size())
		)
	);

	for (Bee* bee : bees_to_catch) {
		this->_catch_bee(bee);
	}
}

void CatchingArea::_catch_bee(Bee* to_catch)
{
	to_catch->get_caught(*this);

	std::cout << "Net contains " << this->bees_in_net() << " bees." << std::endl;
}

vector<Bee*> CatchingArea::_get_bees_in_net_range()
{
	vector<Bee*> bees_in_range;

	Sphere net_sphere(this->_beekeeper->position, this->_radius);

	for (Bee* bee : this->_beekeeper->_graph->bees) {
		Sphere bee_sphere(bee->pos, bee->radius);

		if (Sphere::do_intersect(net_sphere, bee_sphere)) {
			bees_in_range.push_back(bee);
		}
	}

	return bees_in_range;
}

size_t CatchingArea::bees_in_net()
{
	return this->caught_bees.size();
}

void CatchingArea::clear()
{
	// We dump bees in BeeField::bees_out_of_game
	vector<Bee*>& target = this->_beekeeper->_graph->bees_out_of_game;
	target.insert(target.end(), this->caught_bees.begin(), this->caught_bees.end());
	this->caught_bees.clear();
}

void CatchingArea::lose_bee()
{
	if (this->caught_bees.size() > 0) {
		this->caught_bees[0]->get_set_free(*this);
	}
}

CoordinateDouble CatchingArea::get_position()
{
	return this->_beekeeper->position;
}

void CatchingArea::reset_size()
{
	this->_radius = this->_net_start_radius;
}