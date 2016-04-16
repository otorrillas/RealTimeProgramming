/* 
    Example of use of Elevator Interface

*/


#include <iostream>
#include <unistd.h>
#include <thread>
#include <string>
#include <vector>

#include "ElevInterface.hpp"
#include "PanelInterface.hpp"


#define OPT_DELAY 175
#define DOOR_DELAY 2000

using namespace std;

ElevInterface elevInt;
PanelInterface panelInt;
int targetFloor = 3, currFloor;

inline void delay( unsigned long ms )
{
    usleep( ms * 1000 );
}


void floor_pause() {
    delay(OPT_DELAY);
    elevInt.set_motor_direction(ElevInterface::DIRN_STOP);
    panelInt.set_door_open_lamp(1);
    delay(DOOR_DELAY);
    panelInt.set_door_open_lamp(0);
}

void workerThread() {
    int tmpFloor;
    while((tmpFloor = elevInt.get_floor_sensor_signal()) != targetFloor) {
        if(tmpFloor != -1) {
            currFloor = tmpFloor;
            panelInt.set_floor_indicator(tmpFloor);
        }
    }
    panelInt.set_floor_indicator(tmpFloor);
    floor_pause();
}

int main() {

    
    cout << "Press STOP button to stop elevator and exit program." << endl;

    currFloor = elevInt.get_floor_sensor_signal();
    if(currFloor == -1) {
        elevInt.set_motor_direction(ElevInterface::DIRN_DOWN);
        while((currFloor = elevInt.get_floor_sensor_signal()) == -1);
        delay(OPT_DELAY);
        elevInt.set_motor_direction(ElevInterface::DIRN_STOP);
    }


    if(currFloor > targetFloor)
        elevInt.set_motor_direction(ElevInterface::DIRN_DOWN);
    else if(currFloor < targetFloor)
        elevInt.set_motor_direction(ElevInterface::DIRN_UP);

    thread wrker(workerThread);
    //wrker.detach();
    
    while (1) {
        if (panelInt.get_stop_signal()) {
            wrker.join();
            elevInt.set_motor_direction(ElevInterface::DIRN_STOP);
            return 0;
        }
    }

}
