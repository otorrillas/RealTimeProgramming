/*
	LiftControl.cpp
	Real-Time Programming Project
	Authors: @bomanmaster @otorrillas
*/

#include "LiftControl.hpp"
#include "ElevInterface.hpp"
#include <vector>

using namespace std;

LiftControl::LiftControl() {
	elevInt = ElevInterface();

	currFloor = elevInt.get_floor_sensor_signal();
	if(currFloor == -1) {
		elevInt.set_motor_direction(ElevInterface::DIRN_DOWN);
		while((currFloor = elevInt.get_floor_sensor_signal()) -1);
		elevInt.set_motor_direction(ElevInterface::DIRN_STOP);
	}
	
}


/** ORDERS **/

void LiftControl::floor_pause() {
	delay(OPT_DELAY);
	elevInt.set_motor_direction(ElevInterface::DIRN_STOP);
	panelInt.set_door_open_lamp(1);
	delay(DOOR_DELAY);
	panelInt.set_door_open_lamp(0);
}

void LiftControl::worker_thread() {
	state = STATE_BUSY;

	bool busy = true;
	while (busy) {
		int nextFloor = -1;
		for(int i = 0; i < N_FLOORS; i++) {
			if(targetFloors[i]) {
				nextFloor = i;
				break;
			}
		}
		if(nextFloor == -1)
			busy = false;
		else {
			elevInt.set_motor_direction(direction);

			while((tmpFloor = elevInt.get_floor_sensor_signal()) != nextFloor) {
    			if(tmpFloor != -1) {
    				currFloor = tmpFloor;
    				panelInt.set_floor_indicator(tmpFloor);
    			}
			}
			floor_pause();
			targetFloors[nextFloor] = false;
		}
	}
	
    state = STATE_IDLE;
    
}

void LiftControl::go_to_floor(int targetFloor) {
	targetFloors[target_floor] = true;
	set_direction(targetFloor);
	thread t_wrk(worker_thread);
	t_wrk.detach();	

}


/** GETTERS AND SETTERS **/

void set_direction(int targetFloor) { 
	targetFloor--;
	if(targetFloor < currFloor)
		direction = ElevInterface::DIRN_DOWN;
	else if (targetFloor > currFloor)
		direction = ElevInterface::DIRN_UP;
	else
		direction = ElevInterface::DIRN_STOP;

}

void LiftControl::set_target_floor(int floor) {
	targetFloors[floor] = true;
}

bool LiftControl::get_state() {
	return state;
}
int LiftControl::get_current_floor() {
	int tmpFloor = elevInt.get_floor_sensor_signal();
	if(tmpFloor >= 0)
		currFloor = tmpFloor;
	return currFloor;
}

