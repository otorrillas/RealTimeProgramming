/*
	LiftControl.hpp
	Real-Time Programming Project
	Authors: @bomanmaster @otorrillas
*/

#include "ElevInterface.hpp"
#include <vector>
#include <thread>

using namespace std;

#define STATE_IDLE 0
#define STATE_BUSY 1

#define OPT_DELAY 175
#define DOOR_DELAY 2000

class LiftControl {
private:
	// State variables
	bool state;	// 0 idle, 1 busy
	int currFloor;

	// Only have sense when state == BUSY
	vector<bool> targetFloors(N_FLOORS, false);
	elev_motor_direction_t direction;
	

	// Elevator & Panel interface
	ElevInterface elevInt;
	PanelInterface panelInt;

public:
	LiftControl();

	void go_to_floor(int floor);

	bool get_state();
	int get_current_floor();
	

private:
	void worker_thread();
	void set_motor_direction(elev_motor_direction_t dirn);
	void set_target_floor(int floor);
	void floor_pause();

}