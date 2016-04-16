/*
	LiftWorker.cpp
	Real-Time Programming Project
	Authors: @bomanmaster @otorrillas
*/

#include "ElevInterface.hpp"
#include "PanelInterface.hpp"

#include <vector>

using namespace std;

ElevInterface elevInt = ElevInterface();
PanelInterface panelInt = PanelInterface();


void floor_pause() {
	delay(OPT_DELAY);
	elevInt.set_motor_direction(ElevInterface::DIRN_STOP);
	panelInt.set_door_open_lamp(1);
	delay(DOOR_DELAY);
	while(panelInt.get_obstruction_signal() == 1);
	panelInt.set_door_open_lamp(0);
}

int main(int argc,char *argv[]) {
	if(argc != 2)
		return -1;

	
	/* INITIAL SETUP */
	int currentFloor = elevInt.get_floor_sensor_signal();
	int targetFloor = argv[1];
	elev_motor_direction_t direction = ElevInterface::DIRN_UP;
	elev_button_type_t targetBtnType = PanelInterface::BUTTON_CALL_UP;
	if (panelInt.get_button_signal(PanelInterface::BUTTON_CALL_DOWN, targetFloor) == 1) {
		direction = ElevInterface::DIRN_DOWN;
		targetBtnType = PanelInterface::BUTTON_CALL_DOWN;
	}


	/* GOING TO TARGET FLOOR */

	if(targetFloor > currentFloor)
		elevInt.set_motor_direction(ElevInterface::DIRN_UP);
	else if (targetFloor < currentFloor)
		elevInt.set_motor_direction(ElevInterface::DIRN_DOWN);

	while(elevInt.get_floor_sensor_signal != targetFloor);
	panelInt.set_button_lamp(PanelInterface::BUTTON_CALL_UP, targetFloor, 0);

	floor_pause();

	/* PROCESSING FLOORS REQUESTS */

	list<int> requestedFloors;
	for(int i = 0; i < N_FLOORS; i++)
		if(panelInt.get_button_signal(PanelInterface::BUTTON_COMMAND, i) == 1)
			requestedFloors.insert(i);
	
	if(direction == 0)
		requestedFloors.reverse();


	while(!requestedFloors.empty()) {
		elevInt.set_motor_direction(direction);
		while(requestedFloors.front() != (currentFloor = elevInt.get_floor_sensor_signal())) {
			if(currentFloor > 0)
				panelInt.set_floor_indicator(currentFloor);
		}
		requestedFloors.pop_front();
		floor_pause();
	}

}