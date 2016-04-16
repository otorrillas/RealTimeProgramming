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
		while((currFloor = elevInt.get_floor_sensor_signal()) == -1);
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

void ClientControl::call_execv(const char *programname, vector<string> args_vector) {
	 
    const char **argv = new const char* [vector.size()+2];   // extra room for program name and sentinel
    argv [0] = programname;         // by convention, argv[0] is program name
    for (unsigned int j = 0;  j < vector.size()+1;  ++j)     // copy args
            argv [j+1] = vector[j] .c_str();

    argv [vector.size()+1] = NULL;  // end of arguments sentinel is NULL

    
    int rc2 = execv (programname, (char **)argv);
		if (rc2 == -1)
			perror("Error at spawning LiftWorker");
}

void ClientControl::set_target_floor(int targetFloor) {
	state = STATE_BUSY;
	pid_t pid = fork();

	if(pid == 0) {
		// making child process independent
		setsid();
		// preparing arguments
		std::vector<string> args = {"-x", "./LiftWorker", to_string(targetFloor)};
		call_execv("/usr/bin/gnome-terminal", args);
		
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