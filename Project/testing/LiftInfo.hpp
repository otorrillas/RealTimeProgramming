/*
	LiftInfo.hpp
	Real-Time Programming Project
	Authors: @bomanmaster @otorrillas
*/
#ifndef _LIFTINFO_HPP
#define _LIFTINFO_HPP

#define DIR_UP 1
#define DIR_DOWN -1
#define DIR_STOP 0

#define STATUS_IDLE 0
#define STATUS_BUSY 1

class LiftInfo
{
public:
	LiftInfo();
	LiftInfo(string id) {
		this->id = id;
	}
	string id;
	int currFloor;
	int direction;
	bool status;

	
};

#endif