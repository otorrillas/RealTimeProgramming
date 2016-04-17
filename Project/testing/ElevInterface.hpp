// Wrapper for libComedi Elevator control.
// These functions provide an interface to the elevators in the real time lab


#define MOTOR_SPEED 2800


class ElevInterface {
public:
	typedef enum tag_elev_motor_direction { 
		DIRN_DOWN = -1,
		DIRN_STOP = 0,
		DIRN_UP = 1
	} elev_motor_direction_t;

	int dirn;


	ElevInterface();
	void set_motor_direction(elev_motor_direction_t dirn);
	int get_direction(void);
	int get_floor_sensor_signal(void);
	
};




