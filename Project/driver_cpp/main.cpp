/* 
    Example of use of Elevator Interface

*/


#include <iostream>
#include "ElevInterface.hpp"

using namespace std;

int main() {
    ElevInterface elevInt;

    cout << "Press STOP button to stop elevator and exit program." << endl;

    elevInt.set_motor_direction(ElevInterface::DIRN_UP);

    while (1) {
        // Change direction when we reach top/bottom floor
        if (elevInt.get_floor_sensor_signal() == N_FLOORS - 1) {
            elevInt.set_motor_direction(ElevInterface::DIRN_DOWN);
        } else if (elevInt.get_floor_sensor_signal() == 0) {
            elevInt.set_motor_direction(ElevInterface::DIRN_UP);
        }

        // Stop elevator and exit program if the stop button is pressed
        if (elevInt.get_stop_signal()) {
            elevInt.set_motor_direction(ElevInterface::DIRN_STOP);
            return 0;
        }
    }
}
