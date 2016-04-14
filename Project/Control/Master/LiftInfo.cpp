/*
	LiftInfo.cpp
	Real-Time Programming Project
	Authors: @bomanmaster @otorrillas
*/

#include "LiftInfo.hpp"

LiftInfo::LiftInfo(string id, int floor) {
	this->id = id;
	this->lfloor = lfloor;
	this->direction = DIR_STOP;
	this->status = STATUS_IDLE;
}

string LiftInfo::get_id() {
	return this->id;
}
int LiftInfo::get_floor() {
	return this->lfloor;
}
int LiftInfo::get_direction() {
	return this->direction;
}
bool LiftInfo::get_status() {
	return this->status;
}

void LiftInfo::set_floor(int lfloor) {
	this->lfloor = lfloor;
}
void LiftInfo::set_direction(int direction) {
	this->direction = direction;
}
void LiftInfo::set_status(bool status) {
	this->status = status;
}
