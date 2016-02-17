/* 
    Example of use of Elevator Interface

*/


#include <iostream>
#include <unistd.h>

#include "ElevInterface.hpp"
#include "PanelInterface.hpp"

using namespace std;

inline void delay( unsigned long ms )
{
usleep( ms * 1000 );
}


int main() {
    ElevInterface elevInt;
    PanelInterface panelInt;

    cout << "Press STOP button to stop elevator and exit program." << endl;

    int currFloor = elevInt.get_floor_sensor_signal();
    if(currFloor == -1) {
        elevInt.set_motor_direction(ElevInterface::DIRN_DOWN);
        while((currFloor = elevInt.get_floor_sensor_signal()) == -1);
        delay(175);
        elevInt.set_motor_direction(ElevInterface::DIRN_STOP);
    }

    int targetFloor = 3;

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

    
    while (1) {
        if (panelInt.get_stop_signal()) {
            elevInt.set_motor_direction(ElevInterface::DIRN_STOP);
            return 0;
        }
    }

}
