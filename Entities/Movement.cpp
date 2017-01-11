#include <iostream>
#include "Movement.h"
#include "Bee.h"

Movement::Movement(Bee& bee)
	: _bee(bee)
{}

Position Movement::check_movement()
{
	this->tag_neighbours();

	Position total_force{ 0,0 };
	Position current_force{ 0,0 };

	current_force = this->fly_away_from_beekeeper(this->_bee._graph->get_beekeeper()->position)
		* 15;
	if (!this->calculate_forces(total_force, current_force)) {
		return total_force;
	}

	current_force = this->fly_away_from_each_other()
		* 85;
	if (!this->calculate_forces(total_force, current_force)) {
		return total_force;
	}

	current_force = this->fly_with_each_other()
		* 1;
	if (!this->calculate_forces(total_force, current_force)) {
		return total_force;
	}

	current_force = this->fly_to_each_other()
		* 0.3;
	if (!this->calculate_forces(total_force, current_force)) {
		return total_force;
	}

	current_force = this->fly_around()
		* 0.1;
	if (!this->calculate_forces(total_force, current_force)) {
		return total_force;
	}

	return total_force;
}

void Movement::tag_neighbours()
{
	this->_neighbours.clear();
	this->_evade_neighbours.clear();

	for (Bee* target : this->_bee._graph->bees) {

		if (target == &this->_bee) {
			continue;
		}

		Position distance = target->pos - this->_bee.pos;

		if (distance.get_length() < this->_bee.max_vision_range) {
			this->_neighbours.push_back(target);
		}
		if (distance.get_length() < this->_bee.bee_evade_range) {
			this->_evade_neighbours.push_back(target);
		}
	}
}

Position Movement::fly_away_from_beekeeper(Position target_pos)
{
	Position desired = (this->_bee.pos - target_pos);

	if (desired.get_length() < this->_bee.max_vision_range * 2) {
		desired = desired.normalise() * this->_bee.max_speed;
		return (desired - this->_bee.velocity);
	}

	return{ 0,0 };
}

Position Movement::fly_away_from_each_other()
{
	Position force = { 0,0 };

	for (auto& neighbour : this->_evade_neighbours) {
		Position distance = this->_bee.pos - neighbour->pos;
		force = force + (distance.normalise() / distance.get_length());
	}

	return force;
}

Position Movement::fly_with_each_other()
{
	Position heading = { 0,0 };

	if (this->_neighbours.size() > 0) {
		for (auto& neighbour : this->_neighbours) {
			heading = heading + neighbour->heading;
		}

		heading = heading / (double)this->_neighbours.size();
		heading = heading - this->_bee.heading;
	}

	return heading;
}

Position Movement::fly_to_each_other()
{
	Position center_of_mass = { 0,0 };
	Position force = { 0,0 };

	if (this->_neighbours.size() > 0) {
		for (auto& neighbour : this->_neighbours) {
			center_of_mass = center_of_mass + neighbour->pos;
		}

		center_of_mass = center_of_mass / (double)this->_neighbours.size();
		force = this->find_location(center_of_mass);
	}

	return force;
}

Position Movement::find_location(Position target_pos)
{
	Position desired = target_pos - this->_bee.pos;
	desired = desired.normalise() * this->_bee.max_speed;

	return (desired - this->_bee.velocity);
}

Position Movement::arrive(Position target_pos)
{
	Position to_target = target_pos - this->_bee.pos;

	double distance = to_target.get_length();

	if (distance > 0) {
		const double deceleration_value = 0.5;

		double speed = distance / deceleration_value;

		speed = (speed > this->_bee.max_speed) ? this->_bee.max_speed : speed;

		Position desired = to_target * speed / distance;

		return desired - this->_bee.velocity;
	}

	return Position{ 0,0 };
}

Position Movement::fly_around()
{
	this->_bee.wander_target =
		this->_bee.wander_target + Position{ this->random_movement() * this->_bee.wander_jitter,
		this->random_movement() * this->_bee.wander_jitter };
	this->_bee.wander_target = this->_bee.wander_target.normalise();

	this->_bee.wander_target = this->_bee.wander_target * this->_bee.wander_radius;

	Position target = this->_bee.wander_target + Position{ this->_bee.wander_distance, 0 };

	return target;
}

double Movement::random_movement()
{ 
	return (double)((rand() % 200) - 100) / 100;
	//return (double)((RANDOM.get_random_int(0, 200) - 100) / 100);
}

bool Movement::calculate_forces(Position& total, Position to_add)
{
	double remaining = this->_bee.max_force - total.get_length();

	if (remaining <= 0.0) {
		return false;
	}

	if (to_add.get_length() < remaining) {
		total = total + to_add;
	}
	else {
		total = total + (to_add.normalise() * remaining);
	}

	return true;
}