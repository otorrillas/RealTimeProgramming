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

	// Communication with master
	void com_initialize();
	void com_listen();
	void com_reply();

	

private:

	// Getters
	bool elev_get_state();
	int elev_get_current_floor();
	int elev_get_target_floors();

	// setters
	void elev_add_target_floor(int floor);
	void elev_set_direction(elev_motor_direction_t dirn);


}