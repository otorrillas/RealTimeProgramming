#include "ElevInterface.hpp"
#include <vector>

using namespace std;

class Lift {
private:
	// State variables
	bool state;
	int currFloor;

	// Only have sense when state == BUSY
	vector<bool> targetFloors(N_FLOORS, false);
	int direction;

	// Communication
	

	// Elevator interface
	ElevInterface elevInt;

public:
	Lift();

	// Getters
	bool get_state();
	int get_current_floor();
	int get_target_floors();

	// setters
	void go_to_floor(int floor);

private:
	
	void set_direction(elev_motor_direction_t dirn);


}