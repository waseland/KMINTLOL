#pragma once

class Beekeeper;

class FSM
{
public:
	FSM(Beekeeper* beekeeper);
	~FSM();

	static const int amount_of_states = 3;
	double ReturnHomeChance;
	double GetPowerUpChance;
	double GetIdleChance;

	int previous_state;
	Beekeeper* _beekeeper;

	int fsm_get_state();
	void recalculate_states();
	void recalculate_states(size_t ticks, int amountofbees);

};

