/*
	ClientControl.cpp
	Real-Time Programming Project
	Authors: @bomanmaster @otorrillas
*/

#include "ClientControl.hpp"

using namespace std;

ClientControl::ClientControl() {
	elevInt = ElevInterface();
	state = STATE_IDLE;
	workerPID = -1;

	currFloor = elevInt.get_floor_sensor_signal();
	if(currFloor == -1) {
		elevInt.set_motor_direction(ElevInterface::DIRN_DOWN);
		while((currFloor = elevInt.get_floor_sensor_signal()) -1);
		elevInt.set_motor_direction(ElevInterface::DIRN_STOP);
	}
	
}


int ClientControl::get_current_floor() {
	int tmpFloor = elevInt.get_floor_sensor_signal();
	if(tmpFloor >= 0)
		currFloor = tmpFloor;
	return currFloor;
}

void ClientControl::update_state() {
	if(workerPID != -1 and (0 == kill(pid, 0)))
		// Worker is still running
		state = STATE_BUSY;
	else 
		state = STATE_IDLE;
}

bool ClientControl::get_state() {
	update_state();
	return state;
}

void ClientControl::set_target_floor(int targetFloor) {
	state = STATE_BUSY;
	pid_t pid = fork();

	if(pid == 0) {
		setsid();

		// Child process
		char *argv2[] = {"LiftWorker", "-x", "./LiftWorker", NULL};
		int rc2 = execv("/usr/bin/gnome-terminal", argv2);
		if (rc2 == -1)
			perror("Error at spawning LiftWorker");
	}
	else if (pid > 0) {
		// parent process
		workerPID = pid;
	}
}

void ClientControl::set_light(int btn_type, int targetFloor, int value) {
	panelInt.set_button_lamp(
		(PanelInterface::tag_elev_lamp_type)btn_type,
		targetFloor,
		value
	);
}

bool ClientControl::is_button_active(int btn_type, int targetFloor) {
	return panelInt.get_button_signal(
		(PanelInterface::tag_elev_lamp_type)btn_type, 
		targetFloor) 
	== 1;
}