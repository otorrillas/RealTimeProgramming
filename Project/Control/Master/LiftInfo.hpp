/*
	LiftInfo.hpp
	Real-Time Programming Project
	Authors: @bomanmaster @otorrillas
*/

using namespace std;

#define DIR_UP 1
#define DIR_DOWN -1
#define DIR_STOP 0

#define STATUS_IDLE false
#define STATUS_BUSY true

class LiftInfo
{
private:
	string id;
	int lfloor;
	int direction;
	bool status;


public:
	LiftInfo(String id);

	string get_id();
	int get_floor();
	int get_direction();
	bool get_status();

	void set_floor(int lfloor);
	void set_direction(int direction);
	void set_status(bool status);

	
};