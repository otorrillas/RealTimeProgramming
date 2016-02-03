

// Number of floors. Hardware-dependent, do not modify.
#define N_FLOORS 4

// Number of buttons (and corresponding lamps) on a per-floor basis
#define N_BUTTONS 3

class PanelInterface {

public:
	typedef enum tag_elev_lamp_type { 
		BUTTON_CALL_UP = 0,
		BUTTON_CALL_DOWN = 1,
		BUTTON_COMMAND = 2
	} elev_button_type_t;

	PanelInterface();

	void set_button_lamp(elev_button_type_t button, int floor, int value);
	void set_floor_indicator(int floor);
	void set_door_open_lamp(int value);
	void set_stop_lamp(int value);

	int get_stop_signal(void);
	int get_obstruction_signal(void);
	int get_button_signal(elev_button_type_t button, int floor);
};