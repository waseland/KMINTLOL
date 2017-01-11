#pragma once

#include "../IGameObject.h"
#include "../Domain/CoordinateDouble.h"
#include <vector>

using std::vector;

class Beekeeper;
class Bee;

class CatchingArea : public IGameObject {
public:
	CatchingArea(Beekeeper* context);

	vector<Bee*> caught_bees;

	virtual void Draw() override;
	virtual void Update(float deltaTime) override;

	void catch_bees_in_range(int max_bees_in_net);

	CoordinateDouble get_position();

	void lose_bee();

	void clear();
	void reset_size();
	size_t bees_in_net();

private:
	const Color _draw_colour = { 0xFF, 0x0, 0x0, 0x80 };
	const int _net_start_radius = 30;
	const int _net_growth = 10;
	const int _net_growth_interval_ms = 1000;

	int _radius;

	int _last_growth_timestamp;

	void _expand();

	void _catch_bee(Bee* to_catch);
	vector<Bee*> _get_bees_in_net_range();
	Beekeeper* _beekeeper;
};