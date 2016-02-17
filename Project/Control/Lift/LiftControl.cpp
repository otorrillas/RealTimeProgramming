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


void LiftControl::busy() {
	
}

void set_floor_direction(int targetFloor) { 
	targetFloor--;
	if(currFloor > targetFloor)
		elevInt.set_motor_direction(ElevInterface::DIRN_DOWN);
	else 
		elevInt.set_motor_direction(ElevInterface::DIRN_UP);

	int tmpFloor;
    while((tmpFloor = elevInt.get_floor_sensor_signal()) != targetFloor)
    	if(tmpFloor != -1) 
    		currFloor = tmpFloor; 
    
    delay(175);
    elevInt.set_motor_direction(ElevInterface::DIRN_STOP);	
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

void LiftControl::set_target_floor(int floor) {
	targetFloors[floor] = true;
}

