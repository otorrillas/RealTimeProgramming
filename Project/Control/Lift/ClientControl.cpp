/*
	ClientControl.cpp
	Real-Time Programming Project
	Authors: @bomanmaster @otorrillas
*/

#include "ClientControl.hpp"
#include "ElevInterface.hpp"
#include "PanelInterface.hpp"

#include <string>
#include <unistd.h>
#include <stdio.h>

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

void update_state() {
	if(workerPID != -1 and (0 == kill(pid, 0)))
		// Worker is still running
		state = STATE_BUSY;
	else 
		STATE_IDLE;
	

}

bool ClientControl::get_state() {
	update_state();
	return state;
}

void ClientControl::set_target_floor(int targetFloor) {
	state = STATE_BUSY;
	pid_t pid = fork();

	if(pid == 0) {
		// Child process
		char *argv2[] = {"LiftWorker", "-x", "./LiftWorker", NULL};

		setsid();
		int rc2 = execv("/usr/bin/gnome-terminal", argv2);
		if (rc2 == -1)
			perror("Error at spawning LiftWorker");
	}
	else if (pid > 0) {
		// parent process
		workerPID = pid;
	}
}