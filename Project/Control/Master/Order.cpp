/*
	Order.cpp
	Real-Time Programming Project
	Authors: @bomanmaster @otorrillas
*/

#include "Order.hpp"

Order::Order(int startFloor) {
	this->startFloor = startFloor;
	this->targetFloor = -1;
}

int Order::get_start_floor() {
	return startFloor;
}
int Order::get_target_floor() {
	return targetFloor;
}

void Order::set_start_floor(int startFloor) {
	this->startFloor = startFloor;
}

void Order::set_target_floor(int targetFloor) {
	this->targetFloor = targetFloor;
}