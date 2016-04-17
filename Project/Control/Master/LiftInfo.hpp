/*
	LiftInfo.hpp
	Real-Time Programming Project
	Authors: @bomanmaster @otorrillas
*/

using namespace std;

#define DIR_UP 1
#define DIR_DOWN -1
#define DIR_STOP 0

#define STATUS_IDLE 0
#define STATUS_BUSY 1

class LiftInfo
{
public:
	LiftInfo(String id, int currFloor) {
		this->id = id;
		this->currFloor = currFloor;
		this->direction = DIR_STOP;
		this->status = STATUS_IDLE;
	}

	string id;
	int currFloor;
	int direction;
	bool status;

	
};