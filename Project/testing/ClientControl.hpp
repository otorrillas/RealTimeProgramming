/*
	ClientControl.hpp
	Real-Time Programming Project
	Authors: @bomanmaster @otorrillas
*/

#include "ElevInterface.hpp"
#include "PanelInterface.hpp"

#include <string>
#include <vector>
#include <thread>
	
#include <unistd.h>
#include <stdio.h>
#include <signal.h>


using namespace std;

#define STATE_IDLE 0
#define STATE_BUSY 1

#define OPT_DELAY 175
#define DOOR_DELAY 2000

class ClientControl {
private:
	// State variables
	int currFloor;
	int direction;
	int workerPID;
	bool state;	// 0 idle, 1 busy
	bool hasChangedDir;

	// Elevator & Panel interface
	ElevInterface elevInt;
	PanelInterface panelInt;

	void update_state();
	void call_execv(const char *programname, vector<string> args_vector);

public:
	ClientControl();
	int get_direction();
	int get_current_floor();
	bool get_state();
	void set_target_floor(int targetFloor);
	bool is_button_active(int btn_type, int targetFloor);
	void set_light(int btn_type, int targetFloor, int value);

};