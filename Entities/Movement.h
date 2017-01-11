#include <vector>
#include "../Vector/Position.h"

class Bee;

using std::vector;
using std::pair;

class Movement {
public:
	Movement(Bee& bee);

	Position check_movement();
	Position fly_away_from_beekeeper(Position target_pos);
	Position fly_away_from_each_other();
	Position fly_with_each_other();
	Position fly_to_each_other();
	Position find_location(Position target_pos);
	Position arrive(Position target_pos);
	Position fly_around();
	bool calculate_forces(Position& total, Position to_add);

private:
	Bee& _bee;

	void tag_neighbours();
	double random_movement();
	vector<Bee*> _neighbours;
	vector<Bee*> _evade_neighbours;
};