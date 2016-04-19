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
#define STATUS_INIT 2
#define STATUS_DISC 3

struct LiftInfo
{
	int currFloor;
	int direction;
	string id;
	bool status;

};

#endif