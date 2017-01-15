#include "FSM.h"
#include <stdlib.h>
#include <iostream>
#include "../Entities/Beekeeper.h"


FSM::FSM(Beekeeper* beekeeper)
: _beekeeper(beekeeper)
{
	ReturnHomeChance = (100 / amount_of_states) / 99.01f;
	GetPowerUpChance = (100 / amount_of_states) / 99.01f;
	GetIdleChance = (100 / amount_of_states) / 99.01f;
	previous_state = 0;
}


FSM::~FSM()
{
}

int FSM::fsm_get_state()
{
	double val = (double)rand() / RAND_MAX;

	int random;
	if (val < ReturnHomeChance) {
		random = 1;
		previous_state = 1;
	}	
	else if (val < ReturnHomeChance + GetPowerUpChance) {
		random = 2;
		previous_state = 2;
	}		
	else if (val < ReturnHomeChance + GetPowerUpChance + GetIdleChance) {
		random = 3;
		previous_state = 3;
	}	
	else {
		random = 0; 
		previous_state = 0;
	}
	return random;
}

void FSM::recalculate_states() 
{
	switch (previous_state) {
	case 1:
		ReturnHomeChance = ReturnHomeChance  - 0.02;
		GetPowerUpChance = GetPowerUpChance  + 0.01;
		GetIdleChance = GetIdleChance + 0.01;
		break;
	case 2:
		ReturnHomeChance = ReturnHomeChance + 0.01;
		GetPowerUpChance = GetPowerUpChance - 0.02;
		GetIdleChance = GetIdleChance + 0.01;
		break;
	case 3:
		ReturnHomeChance = ReturnHomeChance + 0.01;
		GetPowerUpChance = GetPowerUpChance + 0.01;
		GetIdleChance = GetIdleChance - 0.02;
		break;
	default:
		break;
	}
}

void FSM::recalculate_states(size_t ticks, int amountofbees)
{
	this->_beekeeper->rounds++;
	double i = (double)amountofbees / ticks;
	this->_beekeeper->total_fitness = this->_beekeeper->total_fitness + i;
	double average_fitness = this->_beekeeper->total_fitness / this->_beekeeper->rounds;
	std::cout << "ticks: " << ticks << " amountofbees: " << amountofbees << " i: " << i << std::endl;
	std::cout << "rounds: " << this->_beekeeper->rounds << " averagefitness: " << average_fitness << " total_fitness: " << this->_beekeeper->total_fitness << std::endl;
	if (i == 0) {
		ReturnHomeChance = ReturnHomeChance + 0.01;
		GetPowerUpChance = GetPowerUpChance + 0.01;
		GetIdleChance = GetIdleChance - 0.02;
		return;
		//idle state was done
	}
	if (i >= average_fitness) {
		ReturnHomeChance = ReturnHomeChance + 0.01;
		GetPowerUpChance = GetPowerUpChance - 0.02;
		GetIdleChance = GetIdleChance + 0.01;
		return;
	}
	else {
		ReturnHomeChance = ReturnHomeChance - 0.02;
		GetPowerUpChance = GetPowerUpChance + 0.01;
		GetIdleChance = GetIdleChance + 0.01;
		return;
	}
}
