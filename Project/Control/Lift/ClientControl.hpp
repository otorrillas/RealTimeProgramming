/*
	ClientControl.hpp
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

class ClientControl {
private:
	// State variables
	bool state;	// 0 idle, 1 busy
	int currFloor;
	int workerPID;

	// Elevator & Panel interface
	ElevInterface elevInt;
	PanelInterface panelInt;

	void update_state();

public:
	ClientControl();
	int get_current_floor();
	bool get_state();
	void set_target_floor(int targetFloor);

}