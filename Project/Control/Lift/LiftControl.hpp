#include "ElevInterface.hpp"
#include <vector>

using namespace std;

class LiftControl {
private:
	// State variables
	bool state;	// 0 idle, 1 busy
	int currFloor;

	// Only have sense when state == BUSY
	vector<bool> targetFloors(N_FLOORS, false);
	int direction;
	

	// Elevator interface
	ElevInterface elevInt;

public:
	LiftControl();

	void go_to_floor(int floor);
	bool get_state();
	
	

private:
	void set_motor_direction(elev_motor_direction_t dirn);

	int get_current_floor();
	void set_target_floor(int floor);

}