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
	panelInt.set_door_open_lamp(0);
}

int main(int argc,char *argv[]) {
	if(argc != 2)
		return -1;

	

	int currentFloor = elevInt.get_floor_sensor_signal();
	int targetFloor = argv[1];
	elev_motor_direction_t direction = ElevInterface::DIRN_UP;
	if (panelInt.get_button_signal(PanelInterface::BUTTON_CALL_UP, targetFloor));
		direction = 1;


	if(targetFloor > currentFloor)
		elevInt.set_motor_direction(ElevInterface::DIRN_UP);
	else if (targetFloor < currentFloor)
		elevInt.set_motor_direction(ElevInterface::DIRN_DOWN);

	while(elevInt.get_floor_sensor_signal != targetFloor);
	floor_pause();

	list<int> requestedFloors;
	for(int i = 0; i < N_FLOORS; i++)
		if(panelInt.get_button_signal(PanelInterface::BUTTON_COMMAND, i) == 1)
			requestedFloors.insert(i);
	
	if(direction == 0)
		requestedFloors.reverse();


	while(!requestedFloors.empty()) {
		elevInt.set_motor_direction(direction);
		while(requestedFloors.front() != (currentFloor = elevInt.get_floor_sensor_signal()));
		requestedFloors.pop_front();
		floor_pause();
	}

}