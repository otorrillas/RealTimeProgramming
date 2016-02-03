/* 
    Example of use of Elevator Interface

*/


#include <iostream>
#include "ElevInterface.hpp"
#include "PanelInterface.hpp"

using namespace std;

int main() {
    ElevInterface elevInt;
    PanelInterface panelInt;

    cout << "Press STOP button to stop elevator and exit program." << endl;

    elevInt.set_motor_direction(ElevInterface::DIRN_UP);

    while (1) {
        // Change direction when we reach top/bottom floor
        int currFloor = elevInt.get_floor_sensor_signal();

        if (currFloor == N_FLOORS - 1) {
            elevInt.set_motor_direction(ElevInterface::DIRN_DOWN);
            panelInt.set_floor_indicator(currFloor);
        } 
        else if (currFloor == 0) {
            elevInt.set_motor_direction(ElevInterface::DIRN_UP);
            panelInt.set_floor_indicator(currFloor);
        }
        else if (currFloor > 0) {
            panelInt.set_floor_indicator(currFloor);
        }

        // Stop elevator and exit program if the stop button is pressed
        if (panelInt.get_stop_signal()) {
            elevInt.set_motor_direction(ElevInterface::DIRN_STOP);
            return 0;
        }
    }
}
